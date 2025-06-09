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

// エリア名を持つ列挙型変数（LineTrace = 0, DoubleLoop = 1, DebrisRemoval = 2, SmartCarry = 3）
enum Area { LineTrace, DoubleLoop, DebrisRemoval, SmartCarry };

class AreaMaster {
 public:
  /**
   * コンストラクタ
   * @param robot ロボット本体の参照
   * @param area エリアの指定(Enum型のArea)
   * @param isLeftCourse コースのLR判定(true:Lコース, false:Rコース)
   * @param targetBrightness 目標輝度
   */
  AreaMaster(Robot& _robot, Area _area, bool _isLeftCourse, int _targetBrightness);

  /**
   * @brief エリアを走行する
   */
  void run();

 private:
  Robot& robot;
  enum Area area;
  bool isLeftCourse;
  int targetBrightness;

  // 各エリアのコマンドファイルベースパス
  static const std::string basePath;

  // コマンドファイル名（各エリア名）
  std::array<std::string, 4> areaCommandNames
      = { "LineTrace", "DoubleLoop", "DebrisRemoval", "SmartCarry" };
};

#endif