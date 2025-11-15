/**
 * @file   AreaMaster.h
 * @brief  エリアを攻略するクラス
 * @author Hara1274
 */

#ifndef AREA_MASTER_H
#define AREA_MASTER_H

#include <vector>
#include <array>
#include "MotionParser.h"

// エリア名を持つ列挙型変数（LineTrace = 0, DoubleLoop = 1, SmartCarry = 2）
enum Area { LineTrace, DoubleLoop, SmartCarry };

class AreaMaster {
 public:
  /**
   * コンストラクタ
   * @param robot Robotインスタンスの参照
   * @param area エリアの指定(Enum型のArea)
   * @param isLeftCourse コースのLR判定(true:Lコース, false:Rコース)
   * @param targetBrightness 目標輝度
   */
  AreaMaster(Robot& _robot, Area _area, bool _isLeftCourse, int _targetBrightness);

  /**
   * @brief エリアを走行する
   */
  void run();

  /**
   * @brief ミニフィグ判定結果に基づくエリアを走行
   */
  void runConditional();

 private:
  Robot& robot;          // Robotインスタンスの参照
  Area area;             // エリアの指定(Enum型のArea)
  bool isLeftCourse;     // コースのLR判定(true:Lコース, false:Rコース)
  int targetBrightness;  // 目標輝度

  // 各エリアのコマンドファイルベースパス
  static const std::string basePath;

  // コマンドファイル名（各エリア名）
  static const std::array<std::string, 3> areaCommandNames;

  /**
   * @brief 動作リストを実行してメモリを解放する
   * @param motionList 実行する動作リスト
   */
  void executeMotions(std::vector<Motion*>& motionList);

  /**
   * @brief 条件分岐用ファイルが存在するかチェック
   * @return true: ファイルが存在する, false: ファイルが存在しない
   */
  bool hasConditionalFiles();

  /**
   * @brief 条件分岐用ファイルパスを生成
   * @param suffix ファイル名に付加するサフィックス（_Common, _FigJudgeSuccess, _FigJudgeFailure）
   * @return 生成されたファイルパス
   */
  std::string getConditionalFilePath(const std::string& suffix);
};

#endif