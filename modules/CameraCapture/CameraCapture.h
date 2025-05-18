/**
 * @file   CameraCapture.h
 * @brief  カメラを制御するクラス
 * @author Hara1274
 */

#ifndef CAMERA_CAPTURE_H
#define CAMERA_CAPTURE_H

#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <chrono>

class CameraCapture {
 public:
  CameraCapture();

  /**
   * @brief 利用可能なカメラIDを探索する
   * @param maxTested 探索する回数の上限
   * @return 利用可能なカメラID（見つからなければ-1）
   */
  int findAvailableCameraID(int maxTested = 10);

  /**
   * @brief クラスの変数に格納されている現在のカメラIDを取得する
   * @return 現在のクラスの変数に格納されているカメラID（見つからなければ-1）
   */
  int getCameraID();

  /**
   * @brief カメラIDをクラスの変数に格納
   * @param id クラスの変数に格納するカメラid
   * @return 無効な値の場合はfalse、成功した場合はtrue
   */
  bool setCameraID(int id);

  /**
   * @brief カメラデバイスをオープンする
   * @return openに失敗した場合はfalse、失敗した場合はtrue
   */
  bool openCamera();

  /**
   * @brief カメラ画像の高さと幅を設定する
   * @param width 設定する画像の幅
   * @param height 設定する画像の高さ
   */
  void setCapProps(double width, double height);

  /**
   * @brief 1枚のカメラ画像を取得する
   * @param outFrame 取得した画像を格納するcv::Mat参照
   * @return 画像取得に成功した場合はtrue、失敗した場合はfalse
   */
  bool getFrame(cv::Mat& outFrame);

  /**
   * @brief 指定した枚数だけ、指定したミリ秒間隔でカメラ画像を取得し、配列に保存する
   * @param frames 取得した画像を格納するcv::Mat型配列（出力用ポインタ）
   * @param numFrames 取得する画像の枚数
   * @param millisecondInterval 画像取得の間隔（ミリ秒）
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

 private:
  cv::VideoCapture cap;
  std::string imgExtension = ".JPEG";
  int cameraID;
};

#endif