/**
 * @file   Snapshot.h
 * @brief  写真を撮影して保存するクラス
 * @author takuchi17
 */

#ifndef SNAPSHOT_H
#define SNAPSHOT_H

#include "Motion.h"
#include "FrameSave.h"

class Snapshot : public Motion {
 public:
  /**
   * コンストラクタ
   * @param _robot ロボットインスタンス
   * @param _fileName ファイル名 (デフォルト: snapshot.JPEG)
   */
  Snapshot(Robot& _robot, const std::string& _fileName = "snapshot");

  /**
   * @brief カメラ撮影を行い、画像を保存する
   */
  void run() override;

 private:
  std::string fileName;           // 保存するファイル名
  static const std::string path;  // 保存するパス
};

#endif
