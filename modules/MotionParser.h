/**
 * @file   MotionParser.h
 * @brief  動作コマンドファイルを解析するクラス
 * @author Hara1274
 */

#ifndef MOTION_PARSER_H
#define MOTION_PARSER_H

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <memory>
#include "StringOperator.h"
#include "Motion.h"
#include "AngleRotation.h"
#include "DistanceStraight.h"
#include "DistanceCameraLineTrace.h"
#include "LineBoundingBoxDetector.h"
#include "ColorStraight.h"
#include "ColorLineTrace.h"
#include "DistanceLineTrace.h"
#include "ColorDistanceLineTrace.h"
#include "EdgeChange.h"
#include "Sleeping.h"
#include "Snapshot.h"
#include "MiniFigCameraAction.h"
#include "BackgroundPlaCameraAction.h"
enum class COMMAND {
  AR,   // 角度指定回頭
  DS,   // 指定距離直進
  CS,   // 指定色直進
  DL,   // 指定距離ライントレース
  DCL,  // 指定距離カメラライントレース
  CL,   // 指定色ライントレース
  CDL,  // 色距離指定ライントレース
  EC,   // エッジ切り替え
  SL,   // 自タスクスリープ
  SS,   // カメラ撮影動作
  MCA,  // ミニフィグのカメラ撮影動作
  BCA,  // 背景のカメラ撮影動作
  NONE
};

class MotionParser {
 public:
  /**
   * @brief ファイルを解析して動作インスタンスのリストを生成する
   * @param robot ロボット本体の参照
   * @param commandFilePath ファイルパス
   * @param targetBrightness 目標輝度
   * @return 動作インスタンスリスト
   */

  static std::vector<Motion*> createMotions(Robot& robot, std::string& commandFilePath,
                                            int targetBrightness);

 private:
  MotionParser();  // インスタンス化を禁止する

  /**
   * @brief 文字列を列挙型COMMANDに変換する
   * @param str 文字列のコマンド
   * @return コマンド
   */
  static COMMAND convertCommand(const std::string& str);

  /**
   * @brief 文字列をbool型に変換する
   * @param command 文字列のコマンド
   * @param stringParameter 文字列のパラメータ
   * @return bool値
   */
  static bool convertBool(const std::string& command, const std::string& stringParameter);
};

#endif
