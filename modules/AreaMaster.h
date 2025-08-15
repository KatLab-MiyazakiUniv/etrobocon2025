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
   * @brief コンストラクタ
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

 private:
  Robot& robot;          // Robotインスタンスの参照
  Area area;             // エリアの指定(Enum型のArea)
  bool isLeftCourse;     // コースのLR判定(true:Lコース, false:Rコース)
  int targetBrightness;  // 目標輝度

  static const std::string basePath;  // 各エリアのコマンドファイルベースパス
  static const std::array<std::string, 3> areaCommandNames;  // コマンドファイル名（各エリア名）
};

#endif