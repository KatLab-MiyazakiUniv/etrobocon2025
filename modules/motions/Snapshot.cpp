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
  while(!robot.getCameraCaptureInstance().getFrame(frame)){ 
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  } 
  // 写真を保存する
  robot.getCameraCaptureInstance().saveFrame(frame, path, fileName);
}
