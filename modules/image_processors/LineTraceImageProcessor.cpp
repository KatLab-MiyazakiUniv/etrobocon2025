/**
 * @file   LineTraceImageProcessor.h
 * @brief  ライントレース用の画像処理クラス
 * @author takuchi17 miyahara046 HaruArima08
 */
#include "LineTraceImageProcessor.h"

LineTraceImageProcessor::LineTraceImageProcessor(const cv::Scalar& _lowerHSV,
                                                 const cv::Scalar& _upperHSV)
  : lowerHSV(_lowerHSV), upperHSV(_upperHSV)
{
}

DetectionResult LineTraceImageProcessor::process(const cv::Mat& frame)
{
  // 注目領域の設定
  // 例: 解像度 640x480 の画像の場合
  const int roi_x = 50;        // 左端からの開始X座標
  const int roi_y = 240;       // 上端からの開始Y座標 (画像の中心より少し下から開始)
  const int roi_width = 540;   // ROIの幅 (例: 640 - 50 - 50 = 540)
  const int roi_height = 240;  // ROIの高さ (例: 480 - 140 = 340, 340に設定)

  cv::Rect roiRect(roi_x, roi_y, roi_width, roi_height);

  // ROIが空でないか、または画像範囲外でないかのチェック
  if(frame.empty()) {
    std::cerr << "Error: Input frame is empty." << std::endl;
    return DetectionResult();
  }
  // ROIが画像サイズを超えないようにクリップする処理
  roiRect = roiRect & cv::Rect(0, 0, frame.cols, frame.rows);
  // クリップした結果ROIが空になった場合
  if(roiRect.empty()) {
    std::cerr << "Error: ROI is empty after clipping." << std::endl;
    return DetectionResult();
  }

  // 入力画像を複製し、ROI領域を赤枠で描画
  cv::Mat debugROIImage = frame.clone();
  cv::rectangle(debugROIImage, roiRect, cv::Scalar(0, 0, 255), 2);  // 赤色(BGR: 0,0,255)
  cv::imwrite("etrobocon2025/datafiles/snapshots/debug_roi.JPEG", debugROIImage);

  // ROIを切り出す
  cv::Mat roiFrame = frame(roiRect);

  // フレームをHSV色空間に変換
  cv::Mat hsvFrame;
  cv::cvtColor(roiFrame, hsvFrame, cv::COLOR_BGR2HSV);

  // HSV範囲でマスクを設定
  cv::Mat mask;
  cv::inRange(hsvFrame, lowerHSV, upperHSV, mask);

  // デバッグ表示
  cv::imwrite("etrobocon2025/datafiles/snapshots/debug1.JPEG", mask);

  // マスクを綺麗にするモルフォロジー処理のためのカーネルを作成
  // サイズは調整が必要。
  cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));

  // オープニング（ノイズ除去）
  cv::morphologyEx(mask, mask, cv::MORPH_OPEN, kernel);

  // クロージング（ラインの結合）
  cv::morphologyEx(mask, mask, cv::MORPH_CLOSE, kernel);

  // デバッグ表示（オプション）
  cv::imwrite("etrobocon2025/datafiles/snapshots/debug2.JPEG", mask);

  DetectionResult result;
  result.wasDetected = false;  // 初期状態は検出失敗

  // マスクの輪郭を検出
  std::vector<std::vector<cv::Point>> contours;
  cv::findContours(mask.clone(), contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

  // 最大輪郭の特定
  double maxArea = 0;
  std::vector<cv::Point> largestContour;
  bool lineFound = false;  // ラインが見つかったかどうかのフラグ

  // 輪郭をイテレート
  for(const auto& contour : contours) {
    double area = cv::contourArea(contour);

    // ある程度の面積がないとノイズとみなす（この閾値は調整）
    // 例: 50ピクセル以上の面積をライン候補とする
    if(area > 50) {
      // 必要に応じてアスペクト比など他のフィルタリングもここに追加
      if(area > maxArea) {
        maxArea = area;
        largestContour = contour;
        lineFound = true;  // 有効なライン候補が見つかった
      }
    }
  }

  // ラインが見つかった場合
  if(lineFound) {
    result.wasDetected = true;
    result.label = "LineTrace";  // ライントレースのラベルを設定

    // 最大輪郭の外接矩形を計算
    cv::Rect boundingBox = cv::boundingRect(largestContour);

    // 矩形の4つの角の座標を設定
    result.topLeft = cv::Point(boundingBox.x + roiRect.x, boundingBox.y + roiRect.y);
    result.topRight
        = cv::Point(boundingBox.x + boundingBox.width + roiRect.x, boundingBox.y + roiRect.y);
    result.bottomLeft
        = cv::Point(boundingBox.x + roiRect.x, boundingBox.y + boundingBox.height + roiRect.y);
    result.bottomRight = cv::Point(boundingBox.x + boundingBox.width + roiRect.x,
                                   boundingBox.y + boundingBox.height + roiRect.y);

    // 元画像にバウンディングボックスを描画（青色）
    cv::Mat debugBoundingBoxImage = frame.clone();
    cv::rectangle(debugBoundingBoxImage, result.topLeft, result.bottomRight, cv::Scalar(255, 0, 0),
                  2);  // 青色(BGR: 255,0,0)
    cv::imwrite("etrobocon2025/datafiles/snapshots/debug_bbox.JPEG", debugBoundingBoxImage);

    // 元画像に輪郭を緑色で描画（塗りつぶしなし）
    cv::Mat debugContoursImage = frame.clone();
    cv::drawContours(debugContoursImage, contours, -1, cv::Scalar(0, 255, 0),
                     2);  // 緑色(BGR: 0,255,0)
    cv::imwrite("etrobocon2025/datafiles/snapshots/debug_contours.JPEG", debugContoursImage);
  }
  return result;
}