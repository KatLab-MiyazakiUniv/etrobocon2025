/**
 * @file   Snapshot.h
 * @brief  サーバーに写真撮影を依頼するクラス
 * @author takuchi17
 */

#ifndef SNAPSHOT_H
#define SNAPSHOT_H

#include "Motion.h"
#include <string>

class Snapshot : public Motion {
 public:
  /**
   * コンストラクタ
   * @param _robot ロボットインスタンス
   * @param _fileName ファイル名 (デフォルト: snapshot.jpeg)
   */
  Snapshot(Robot& _robot, const std::string& _fileName = "snapshot");

  /**
   * @brief サーバーに撮影を依頼する
   */
  void run() override;

 private:
  std::string fileName;  // 保存するファイル名
};

#endif