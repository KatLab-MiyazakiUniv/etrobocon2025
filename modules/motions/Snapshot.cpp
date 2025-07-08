/**
 * @file   Snapshot.cpp
 * @brief  写真を撮影して保存するクラス
 * @author takuchi17
 */

#include "Snapshot.h"

const std::string Snapshot::path = "etrobocon2025/datafiles/snapshots/";

Snapshot::Snapshot(Robot& _robot, const std::string& _fileName)
  : Motion(_robot), fileName(_fileName)
{
}

void Snapshot::run()
{
  // 写真を撮影する
  cv::Mat frame;
  if(!robot.getCameraCaptureInstance().getFrame(frame)) {
    return;
  }
  // 写真を保存する
  FrameSave::frameSave(frame, path, fileName);
}
