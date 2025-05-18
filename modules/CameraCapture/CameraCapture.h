/**
 * @file   AreaACameraAction.cpp
 * @brief  カメラを制御するクラス
 * @author Hara1274
 */

#ifndef CAMERACAPTURE_H
#define CAMERACAPTURE_H

#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <chrono>

class CameraCapture {
 private:
  cv::VideoCapture cap;
  int cameraID;

 public:
  CameraCapture();

  /**
   * @brief　利用可能なカメラIDを探索する
   * @return 利用可能なカメラID（見つからなければ-1）
   */
  int findAvailableCameraID(int maxTested = 10);

  /**
   * @brief　クラスの変数に格納されている現在のカメラIDを取得する
   */
  int getCurrentCameraID();

  /**
   * @brief カメラIDをクラスの変数に格納
   */
  bool setCameraID(int id);

  /**
   * @brief カメラデバイスをオープンする
   */
  bool openCamera();

  /**
   * @brief カメラ画像の高さと幅を設定する
   */
  void setCapProps(double width, double height);

  /**
   * @brief 1枚のカメラ画像を取得する
   */
  bool getFrame(cv::Mat& outFrame);

  /**
   * @brief　指定した枚数だけ、指定したミリ秒間隔でカメラ画像を取得し、配列に保存する
   * @return 全てのフレーム取得に成功した場合はtrue、1つでも失敗した場合はfalse
   */
  bool getFrames(cv::Mat* frames, int numFrames, int millisecondInterval);

  /**
   * @brief 画像をファイルに保存する
   * @param frame 保存する画像
   * @param filepath 保存先ディレクトリのパス
   * @param filename 保存するファイル名（拡張子なし）
   * @return 保存に成功した場合はtrue、失敗した場合はfalse
   */
  bool saveFrame(const cv::Mat& frame, const std::string filepath, const std::string filename);
};

#endif