/**
 * @file   LineTraceImageProcessor.h
 * @brief  ライントレース用の画像処理クラス
 * @author takuchi17 miyahara046
 */
#include "LineTraceImageProcessor.h"

LineTraceImageProcessor::LineTraceImageProcessor(const cv::Scalar& _lowerHSV,
                                                 const cv::Scalar& _upperHSV)
  : lowerHSV(_lowerHSV), upperHSV(_upperHSV)
{
}

DetectionResult LineTraceImageProcessor::process(const cv::Mat& frame)
{
  DetectionResult result;

  // フレームをHSV色空間に変換
  cv::Mat hsvFrame;
  cv::cvtColor(frame, hsvFrame, cv::COLOR_BGR2HSV);

  // HSV範囲でマスクを作成
  cv::Mat mask;
  cv::inRange(hsvFrame, lowerHSV, upperHSV, mask);

  // マスクを綺麗にするためのモルフォロジー処理
  cv::morphologyEx(mask, mask, cv::MORPH_CLOSE, cv::Mat::ones(3, 3, CV_8U));

  // マスクの輪郭を検出
  std::vector<std::vector<cv::Point>> contours;
  cv::findContours(mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

  if(!contours.empty()) {
    // 最大の輪郭を特定
    std::vector<std::vector<cv::Point>>::iterator largestContour;
    largestContour
        = std::max_element(contours.begin(), contours.end(),
                           [](const std::vector<cv::Point>& c1, const std::vector<cv::Point>& c2) {
                             return cv::contourArea(c1) < cv::contourArea(c2);
                           });

    // 最大輪郭の外接矩形を計算
    cv::Rect boundingBox = cv::boundingRect(*largestContour);

    // DetectionResultのヘッダファイル定義を活用して結果を格納
    result.wasDetected = true;
    result.label = "LineTrace";  // ライントレースのラベルを設定

    // 矩形の4つの角の座標を設定
    result.topLeft = cv::Point(boundingBox.x, boundingBox.y);
    result.topRight = cv::Point(boundingBox.x + boundingBox.width, boundingBox.y);
    result.bottomLeft = cv::Point(boundingBox.x, boundingBox.y + boundingBox.height);
    result.bottomRight
        = cv::Point(boundingBox.x + boundingBox.width, boundingBox.y + boundingBox.height);

  } else {
    result.wasDetected = false;  // ラインが検出されない場合
    result.label = "";           // 検出されない場合は空のラベル
  }

  return result;
}