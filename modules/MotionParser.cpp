/**
 * @file   MotionParser.cpp
 * @brief  動作コマンドファイルを解析するクラス
 * @author Hara1274
 */

#include "MotionParser.h"

using namespace std;

std::vector<Motion*> MotionParser::createMotions(Robot& robot, string& commandFilePath,
                                                 int targetBrightness)

{
  // 消す
  bool isLeftEdge = true;

  // ファイルの行番符号
  int lineNum = 1;

  // 動作インスタンスのリスト
  vector<Motion*> motionList;

  // ファイル読み込み。失敗したらエラーメッセージを表示して終了
  FILE* fp = fopen(commandFilePath.c_str(), "r");
  if(fp == NULL) {
    cout << commandFilePath << "file not open!" << endl;
    return motionList;
  }

  // 1行分の文字列を保持する配列と、区切り文字（カンマ）を定義
  char row[READ_BUF_SIZE];
  const char* separator = ",";

  // ファイルを1行ずつ読み込む
  while(fgets(row, READ_BUF_SIZE, fp) != NULL) {
    // 1行のコマンドと引数の項目を格納する配列
    vector<char*> params;

    // 1行の文字列をカンマで区切り、paramで順番に各項目を取り出してparamsに格納する
    char* param = strtok(row, separator);
    while(param != NULL) {
      params.push_back(param);
      param = strtok(NULL, separator);
    }

    // paramsの0番目はコマンド名なので、それを使って対応する動作コマンドに変換
    COMMAND command = convertCommand(params[0]);

    // コマンドに応じて対応する動作オブジェクトを生成し、動作リスト（motionList）に追加する処理

    switch(command) {
      case COMMAND::AR: {
        // =====  角度指定回頭（AngleRotation） =====
        // params[1] : 目標回転角度(deg) 0~360
        // params[2] : 指定する速度（mm/秒）
        // params[3] : 回頭方向 true:時計回り, false:反時計回り

        AngleRotation* ar = new AngleRotation(robot, atoi(params[1]), atoi(params[2]),
                                              convertBool(params[0], params[3]));
        motionList.push_back(ar);
        break;
      }

      case COMMAND::DS: {
        // =====  指定距離直進（DistanceStraight） =====
        // params[1] : 目標距離 [mm]
        // params[2] : 目標速度[mm/s]

        DistanceStraight* ds = new DistanceStraight(robot, atoi(params[1]), atoi(params[2]));
        motionList.push_back(ds);
        break;
      }

      case COMMAND::CS: {
        // ===== 指定色直進（ColorStraight） =====
        // params[1] : 目標色（文字列）
        // params[2] : 目標速度[mm/s]

        ColorStraight* cs = new ColorStraight(robot, ColorJudge::convertStringToColor(params[1]),
                                              atof(params[2]));
        motionList.push_back(cs);
        break;
      }

      case COMMAND::DL: {
        // ===== 指定距離ライントレース（DistanceLineTracing） =====
        // params[1] : 目標距離 [mm]
        // params[2] : 目標速度[mm/s]
        // params[3] : 輝度補正値（targetBrightness に加算するオフセット値）
        // params[4] : Pゲイン
        // params[5] : Iゲイン
        // params[6] : Dゲイン

        DistanceLineTrace* dl = new DistanceLineTrace(
            robot, atof(params[1]), atof(params[2]), targetBrightness + atoi(params[3]),
            PidGain(atof(params[4]), atof(params[5]), atof(params[6])), isLeftEdge);

        motionList.push_back(dl);  // 動作リストに追加

        break;
      }

      default: {
        // 未定義のコマンド
        cout << commandFilePath << ":" << lineNum << " コマンド " << params[0] << " は未定義です。"
             << endl;
        break;
      }
    }
    // 行番号をインクリメントする
    lineNum++;
  }
  // ファイルを閉じる
  fclose(fp);

  return motionList;
}

COMMAND MotionParser::convertCommand(const string& str)
{
  if(str == "AR") {  // 角度指定回答
    return COMMAND::AR;
  } else if(str == "DS") {  // 指定距離直進
    return COMMAND::DS;
  } else if(str == "CS") {  // 指定色直進
    return COMMAND::CS;
  } else if(str == "DL") {  // 指定距離ライントレース
    return COMMAND::DL;
  } else if(str == "CL") {  // 指定色ライントレース
    return COMMAND::CL;
  } else if(str == "CDL") {  // 色距離指定ライントレース
    return COMMAND::CDL;
  } else if(str == "EC") {  // エッジ切り替え
    return COMMAND::EC;
  } else if(str == "SL") {  // 自タスクスリープ
    return COMMAND::SL;
  } else if(str == "SM") {  // 両輪モーターリセット
    return COMMAND::SM;
  } else if(str == "RM") {  // 両輪モーターリセット
    return COMMAND::RM;
  } else if(str == "CA") {  // カメラ撮影動作
    return COMMAND::CA;
  } else {
    return COMMAND::NONE;  // 一致しない場合
  }
}

bool MotionParser::convertBool(const string& command, const string& stringParameter)
{
  // 末尾の改行を削除
  string param = StringOperator::removeEOL(stringParameter);

  // 回転動作(AR)の場合、"clockwise"ならtrue（時計回り）、"anticlockwise"ならfalse（反時計回り）に変換
  if(command == "AR") {
    if(param == "clockwise") {
      return true;
    } else if(param == "anticlockwise") {
      return false;
    } else {
      cout << "Parameter before conversion must be 'clockwise' or 'anticlockwise'" << endl;
      return true;  // デフォルトtrueにしてる場合
    }
  }

  // エッジ切り替え(EC)の場合、"left"ならtrue（左エッジ）、"right"ならfalse（右エッジ)に変換
  if(command == "EC") {
    if(param == "left") {
      return true;
    } else if(param == "right") {
      return false;
    } else {
      cout << "Parameter before conversion must be 'left' or 'right'" << endl;
      return true;
    }
  }

  // ここまでに条件を満たしてreturnしていなかった場合のデフォルト値としてtrueを返す
  cout << "convertBool function received an unrecognized command: '" << command << endl;
  return true;
}
