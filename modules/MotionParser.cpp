/**
 * @file   MotionParser.cpp
 * @brief  動作コマンドファイルを解析するクラス
 * @author Hara1274
 */

#include "MotionParser.h"

using namespace std;

vector<Motion*> MotionParser::createMotions(Robot& robot, string& commandFilePath,
                                            int targetBrightness)
{
  // 行番号カウンタ
  int lineNum = 1;
  // 動作インスタンスのリスト
  vector<Motion*> motionList;

  // ファイルを開き、開けなければ空のリストを返す
  ifstream file(commandFilePath);
  if(!file) {
    cout << "コマンドファイルを開けませんでした: " << commandFilePath << endl;
    return motionList;
  }

  // 各行を格納する変数と、区切り文字としてカンマを定義
  string line;
  constexpr char separator = ',';

  // fileから1行ずつ文字列として line に読み込む
  while(getline(file, line)) {
    // 文字列 line をストリームに変換
    stringstream ss(line);

    // カンマ区切りでコマンド名とその引数を1つずつ取り出して params に追加
    vector<string> params;
    for(string token; getline(ss, token, separator);) {
      params.push_back(move(token));
    }

    // コマンド名(paramsの0番目)を対応する動作コマンドに変換
    COMMAND command = convertCommand(params[0]);

    // コマンドに応じて対応する動作オブジェクトを生成し、動作リスト（motionList）に追加する処理
    switch(command) {
      // AR: 角度指定回頭
      // [1]:int 角度[deg], [2]:double 速度[mm/s], [3]:string 方向(clockwise or anticlockwise)
      case COMMAND::AR: {
        AngleRotation* ar = new AngleRotation(robot, stoi(params[1]), stod(params[2]),
                                              convertBool(params[0], params[3]));
        motionList.push_back(ar);
        break;
      }

      // DS: 指定距離直進
      // [1]:double 距離[mm], [2]:double 速度[mm/s]
      case COMMAND::DS: {
        DistanceStraight* ds = new DistanceStraight(robot, stod(params[1]), stod(params[2]));
        motionList.push_back(ds);
        break;
      }

      // CS: 指定色直進
      // [1]:string 色, [2]:double 速度[mm/s]
      case COMMAND::CS: {
        ColorStraight* cs = new ColorStraight(robot, ColorJudge::convertStringToColor(params[1]),
                                              stod(params[2]));
        motionList.push_back(cs);
        break;
      }

      // DL: 指定距離ライントレース
      // [1]:double 距離[mm], [2]:double 速度[mm/s], [3]:int 輝度補正, [4-6]:double PIDゲイン
      case COMMAND::DL: {
        DistanceLineTrace* dl = new DistanceLineTrace(
            robot, stod(params[1]), stod(params[2]), targetBrightness + stoi(params[3]),
            PidGain(stod(params[4]), stod(params[5]), stod(params[6])));
        motionList.push_back(dl);
        break;
      }

      // DCL: 指定距離カメラライントレース
      // [1]:double 距離[mm], [2]:double 速度[mm/s], [3]:int X座標[px], [4-6]:double PIDゲイン,
      // [7-9]int lowerHSV, [10-12]int upperHSV,
      // [13-16]int ROI座標[px] ([13]左上隅のx座標, [14]左上隅のy座標, [15]幅, [16]高さ),
      // [17-18]int 解像度[px] ([17]幅, [18]高さ)
      // 補足：ROI（Region of Interest:ライントレース用の画像内注目領域（四角形））
      case COMMAND::DCL: {
        cv::Scalar lowerHSV, upperHSV;
        cv::Rect roi;
        cv::Size resolution;
        std::unique_ptr<BoundingBoxDetector> detector;

        lowerHSV = cv::Scalar(stoi(params[7]), stoi(params[8]), stoi(params[9]));
        upperHSV = cv::Scalar(stoi(params[10]), stoi(params[11]), stoi(params[12]));

        // パラメータ配列のサイズによってコンストラクタを切り替え
        if(params.size() > 18) {
          // ROI + 解像度
          roi = cv::Rect(stoi(params[13]), stoi(params[14]), stoi(params[15]), stoi(params[16]));
          resolution = cv::Size(stoi(params[17]), stoi(params[18]));
          detector = std::make_unique<LineBoundingBoxDetector>(lowerHSV, upperHSV, roi, resolution);
        } else if(params.size() > 16) {
          // ROIのみ
          roi = cv::Rect(stoi(params[13]), stoi(params[14]), stoi(params[15]), stoi(params[16]));
          detector = std::make_unique<LineBoundingBoxDetector>(lowerHSV, upperHSV, roi);
        } else {
          // HSVのみ
          detector = std::make_unique<LineBoundingBoxDetector>(lowerHSV, upperHSV);
        }

        auto dcl = new DistanceCameraLineTrace(
            robot, stod(params[1]), stod(params[2]), stoi(params[3]),
            PidGain(stod(params[4]), stod(params[5]), stod(params[6])), std::move(detector));
        motionList.push_back(dcl);
        break;
      }

      // CL: 指定色ライントレース
      // [1]:string 色, [2]:double 速度[mm/s], [3]:int 輝度補正, [4-6]:double PIDゲイン
      case COMMAND::CL: {
        ColorLineTrace* cl
            = new ColorLineTrace(robot, ColorJudge::convertStringToColor(params[1]),
                                 stod(params[2]), targetBrightness + stoi(params[3]),
                                 PidGain(stod(params[4]), stod(params[5]), stod(params[6])));
        motionList.push_back(cl);
        break;
      }

      // CDL: 色距離指定ライントレース
      // [1]:string 色, [2]:double 距離[mm], [3]:double 速度[mm/s], [4]:int 輝度補正,
      // [5-7]:double PIDゲイン
      case COMMAND::CDL: {
        ColorDistanceLineTrace* cdl = new ColorDistanceLineTrace(
            robot, ColorJudge::convertStringToColor(params[1]), stod(params[2]), stod(params[3]),
            targetBrightness + stoi(params[4]),
            PidGain(stod(params[5]), stod(params[6]), stod(params[7])));
        motionList.push_back(cdl);
        break;
      }

      // EC: エッジ切り替え
      // [1]:string 切り替え後エッジ (left or right)
      case COMMAND::EC: {
        EdgeChange* ec = new EdgeChange(robot, convertBool(params[0], params[1]));
        motionList.push_back(ec);
        break;
      }

      // SL: 自タスクスリープ
      // [1]:double 時間(マイクロ秒)[μs]
      case COMMAND::SL: {
        Sleeping* sl = new Sleeping(robot, stod(params[1]));
        motionList.push_back(sl);
        break;
      }

      // SS: カメラ撮影動作
      // [1]:string ファイル名(デフォルトではsnapshot.JPEG)
      case COMMAND::SS: {
        Snapshot* ss;
        if(params.size() == 2) {
          ss = new Snapshot(robot, params[1]);
        } else {
          ss = new Snapshot(robot);
        }
        motionList.push_back(ss);
        break;
      }

        // MCA: ミニフィグのカメラ撮影動作
        // [1]:int フロントカメラをミニフィグに向けるための回頭角度[deg],
        // [2]:int 黒線復帰のための回頭角度[deg],
        // [3]:double 撮影前後の回頭のための目標速度[mm/s],
        // [4]:double 撮影前の後退距離[mm],
        // [5]:double 撮影後の前進距離[mm],
        // [6]:double 撮影前の後退速度の絶対値[mm/s],
        // [7]:double 撮影後の前進速度の絶対値[mm/s],
        // [8]:string 回頭の方向(clockwise or anticlockwise),
        // [9]:int 撮影位置(0が初期位置)
      case COMMAND::MCA: {
        MiniFigCameraAction* mca = new MiniFigCameraAction(
            robot, convertBool(params[0], params[8]), stoi(params[1]), stoi(params[2]),
            stod(params[3]), stod(params[4]), stod(params[5]), stod(params[6]), stod(params[7]),
            stoi(params[9]));
        motionList.push_back(mca);

        break;
      }

      // CRA: カメラ復帰動作
      // [1]:int 回頭角度[deg], [2]:double 回頭スピード[mm/s], [3]:string 回頭の方向(clockwise or
      // anticlockwise), [4-9]:int HSV値(lowerH,lowerS,lowerV,upperH,upperS,upperV), [10-13]int
      // ROI座標[px]
      // ([10]左上隅のx座標, [11]左上隅のy座標, [12]幅, [13]高さ), [14-15]int 解像度[px] ([14]幅,
      // [15]高さ)
      // 補足：ROI（Region of Interest: ライントレース用の画像内注目領域（四角形））
      case COMMAND::CRA: {
        cv::Scalar lowerHSV, upperHSV;
        cv::Rect roi;
        cv::Size resolution;
        std::unique_ptr<BoundingBoxDetector> boundingBoxDetector;

        lowerHSV = cv::Scalar(stoi(params[4]), stoi(params[5]), stoi(params[6]));
        upperHSV = cv::Scalar(stoi(params[7]), stoi(params[8]), stoi(params[9]));

        // パラメータ配列のサイズによってコンストラクタを切り替え
        if(params.size() > 15) {
          // ROI + 解像度
          roi = cv::Rect(stoi(params[10]), stoi(params[11]), stoi(params[12]), stoi(params[13]));
          resolution = cv::Size(stoi(params[14]), stoi(params[15]));
          boundingBoxDetector
              = std::make_unique<LineBoundingBoxDetector>(lowerHSV, upperHSV, roi, resolution);
        } else if(params.size() > 13) {
          // ROIのみ
          roi = cv::Rect(stoi(params[10]), stoi(params[11]), stoi(params[12]), stoi(params[13]));
          boundingBoxDetector = std::make_unique<LineBoundingBoxDetector>(lowerHSV, upperHSV, roi);
        } else {
          // HSVのみ
          boundingBoxDetector = std::make_unique<LineBoundingBoxDetector>(lowerHSV, upperHSV);
        }

        auto cra = new CameraRecoveryAction(robot, stoi(params[1]), stod(params[2]),
                                            convertBool(params[0], params[3]),
                                            std::move(boundingBoxDetector));
        motionList.push_back(cra);
        break;
      }
      // 未定義コマンド
      default: {
        cout << commandFilePath << ":" << lineNum << " Command " << params[0] << " は未定義です"
             << endl;
        break;
      }
    }

    lineNum++;  // 行番号をインクリメントする
  }

  return motionList;
}

COMMAND MotionParser::convertCommand(const string& str)
{
  // コマンド文字列(string)と、それに対応する列挙型COMMANDのマッピングを定義
  static const unordered_map<string, COMMAND> commandMap = {
    { "AR", COMMAND::AR },    // 角度指定回頭
    { "DS", COMMAND::DS },    // 指定距離直進
    { "CS", COMMAND::CS },    // 指定色直進
    { "DL", COMMAND::DL },    // 指定距離ライントレース
    { "DCL", COMMAND::DCL },  // 指定距離カメラライントレース
    { "CL", COMMAND::CL },    // 指定色ライントレース
    { "CDL", COMMAND::CDL },  // 色距離指定ライントレース
    { "EC", COMMAND::EC },    // エッジ切り替え
    { "SL", COMMAND::SL },    // スリープ
    { "SS", COMMAND::SS },    // カメラ撮影動作
    { "MCA", COMMAND::MCA },  // ミニフィグのカメラ撮影動作
    { "CRA", COMMAND::CRA }   // カメラ復帰動作
  };

  // コマンド文字列に対応するCOMMAND値をマップから取得。なければCOMMAND::NONEを返す
  auto it = commandMap.find(str);
  if(it != commandMap.end()) {
    return it->second;
  } else {
    return COMMAND::NONE;
  }
}

bool MotionParser::convertBool(const string& command, const string& stringParameter)
{
  // 末尾の改行を削除
  string param = StringOperator::removeEOL(stringParameter);

  // 回転動作(AR,MCA,CRA)の場合、"clockwise"ならtrue（時計回り）、"anticlockwise"ならfalse（反時計回り）に変換
  if(command == "AR" || command == "MCA" || command == "CRA") {
    if(param == "clockwise") {
      return true;
    } else if(param == "anticlockwise") {
      return false;
    } else {
      cout << "'clockwise' か 'anticlockwise'を入力してください" << endl;
      return true;
    }
  }

  // エッジ切り替え(EC)の場合、"left"ならtrue（左エッジ）、"right"ならfalse（右エッジ)に変換
  if(command == "EC") {
    if(param == "left") {
      return true;
    } else if(param == "right") {
      return false;
    } else {
      cout << "'left' か 'right'を入力してください" << endl;
      return true;
    }
  }

  // ここまでに条件を満たしていなかった場合は、デフォルト値としてtrueを返す
  cout << "convertBool関数の処理の対象外です: '" << command << endl;
  return true;
}
