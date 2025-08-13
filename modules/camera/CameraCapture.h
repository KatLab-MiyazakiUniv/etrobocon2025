/**
 * @file   CameraCapture.h
 * @brief  カメラを制御するクラス
 * @author Hara1274
 */

#ifndef CAMERA_CAPTURE_H
#define CAMERA_CAPTURE_H

#include "ICameraCapture.h"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <chrono>

class CameraCapture : public ICameraCapture {
 public:
  /**
   * @brief コンストラクタ
   */
  CameraCapture();
  /**
   * @brief デストラクタ
   */
  ~CameraCapture();

  /**
   * @brief 利用可能なカメラIDを探索する
   * @param maxTested 探索する回数の上限
   * @return 利用可能なカメラID（見つからなければ-1）
   */
  int findAvailableCameraID(int maxTested = 10) override;

  /**
   * @brief クラスの変数に格納されている現在のカメラIDを取得する
   * @return 現在のクラスの変数に格納されているカメラID
   */
  int getCameraID() override;

  /**
   * @brief カメラIDをクラスの変数に格納
   * @param id クラスの変数に格納するカメラid
   * @return 成功した場合はtrue、無効な値の場合はfalse
   */
  bool setCameraID(int id) override;

  /**
   * @brief カメラデバイスをオープンする
   * @return openに成功した場合はtrue、失敗した場合はfalse、
   */
  bool openCamera() override;

  /**
   * @brief カメラ画像の高さと幅を設定する
   * @param width 設定する画像の幅
   * @param height 設定する画像の高さ
   */
  void setCapProps(double width, double height) override;

  /**
   * @brief 1枚のカメラ画像を取得する
   * @param outFrame 取得した画像を格納するcv::Mat参照
   * @return 画像取得に成功した場合はtrue、失敗した場合はfalse
   */
  bool getFrame(cv::Mat& outFrame) override;

  /**
   * @brief 指定した枚数だけ、指定したミリ秒間隔でカメラ画像を取得し、配列に保存する
   * @param frames 取得した画像を格納するstd::vector<cv::Mat>型の参照
   * @param numFrames 取得する画像の枚数
   * @param millisecondInterval 画像取得の間隔（ミリ秒）
   * @return 全てのフレーム取得に成功した場合はtrue、1つでも失敗した場合はfalse
   */
  bool getFrames(std::vector<cv::Mat>& frames, int numFrames, int millisecondInterval) override;

 private:
  cv::VideoCapture cap;  // カメラキャプチャオブジェクト
  int cameraID;          // カメラID
};

#endif