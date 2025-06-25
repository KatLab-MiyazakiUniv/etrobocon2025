/**
 * @file   LineBoundingBoxDetector.h
 * @brief  ライントレース用の画像処理クラス
 * @author takuchi17 miyahara046 HaruArima08
 */
#include "LineBoundingBoxDetector.h"

// ROIと解像度を指定するメインコンストラクタ
LineBoundingBoxDetector::LineBoundingBoxDetector(const cv::Scalar& _lowerHSV,
                                                 const cv::Scalar& _upperHSV, const cv::Rect& _roi,
                                                 const cv::Size& _resolution)
  : lowerHSV(_lowerHSV), upperHSV(_upperHSV), roi(_roi), resolution(_resolution)
{
  validateParameters();
}

// ROIを指定するコンストラクタ
LineBoundingBoxDetector::LineBoundingBoxDetector(const cv::Scalar& _lowerHSV,
                                                 const cv::Scalar& _upperHSV, const cv::Rect& _roi)
  : LineBoundingBoxDetector(_lowerHSV, _upperHSV, _roi, cv::Size(640, 480))
{
}

// 最小限のコンストラクタ
LineBoundingBoxDetector::LineBoundingBoxDetector(const cv::Scalar& _lowerHSV,
                                                 const cv::Scalar& _upperHSV)
  : LineBoundingBoxDetector(_lowerHSV, _upperHSV, cv::Rect(50, 240, 540, 240), cv::Size(640, 480))
{
}

void LineBoundingBoxDetector::validateParameters()
{
  // 解像度の検証：設定された解像度が定義した最小・最大値内にあるかの確認
  if(resolution.width < MIN_WIDTH) {
    resolution.width = MIN_WIDTH;
  } else if(resolution.width > MAX_WIDTH) {
    resolution.width = MAX_WIDTH;
  }
  if(resolution.height < MIN_HEIGHT) {
    resolution.height = MIN_HEIGHT;
  } else if(resolution.height > MAX_HEIGHT) {
    resolution.height = MAX_HEIGHT;
  }

  // ROI検証：ROIの位置とサイズが解像度の枠内に収まるかの確認
  // ROI左上座標が負の場合は0にクリップ
  if(roi.x < 0) roi.x = 0;
  if(roi.y < 0) roi.y = 0;
  // ROIが解像度を超えている場合は解像度に合わせる
  if(roi.x + roi.width > resolution.width) roi.width = resolution.width - roi.x;
  if(roi.y + roi.height > resolution.height) roi.height = resolution.height - roi.y;
}

void LineBoundingBoxDetector::detect(const cv::Mat& frame, BoundingBoxDetectionResult& result)
{
  result.wasDetected = false;  // 初期状態は検出失敗

  if(frame.empty()) {
    std::cerr << "Error: Input frame is empty." << std::endl;
    return;
  }

  // 入力画像が指定の解像度と異なる場合、リサイズ処理
  cv::Mat frameProcessed = frame;
  if(frame.size() != resolution) {
    cv::resize(frame, frameProcessed, resolution);
  } else {
    frameProcessed = frame.clone();
  }

  // ROIが画像サイズを超えないようにクリップする処理
  cv::Rect roiRect = roi;
  roiRect = roiRect & cv::Rect(0, 0, frameProcessed.cols, frameProcessed.rows);
  // クリップした結果ROIが空になった場合
  if(roiRect.empty()) {
    std::cerr << "Error: ROI is empty after clipping." << std::endl;
    return;
  }

  // ROIを切り出す
  cv::Mat roiFrame = frameProcessed(roiRect);

  // フレームをHSV色空間に変換
  cv::Mat hsvFrame;
  cv::cvtColor(roiFrame, hsvFrame, cv::COLOR_BGR2HSV);

  // HSV範囲でマスクを設定
  cv::Mat mask;
  cv::inRange(hsvFrame, lowerHSV, upperHSV, mask);

  // マスクを綺麗にするモルフォロジー処理のためのカーネルを作成
  // カーネルサイズは小さすぎるとノイズが除去されず、大きすぎるとラインの細部が失われるため
  // ノイズの大きさやラインの太さに応じて調整が必要。
  cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));

  // オープニング（ノイズ除去）
  cv::morphologyEx(mask, mask, cv::MORPH_OPEN, kernel);

  // クロージング（ラインの結合）
  cv::morphologyEx(mask, mask, cv::MORPH_CLOSE, kernel);

  // マスクの輪郭を検出
  std::vector<std::vector<cv::Point>> contours;
  cv::findContours(mask.clone(), contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

  // 最大輪郭の特定
  double maxArea = 0;
  std::vector<cv::Point> largestContour;

  // 輪郭をイテレート
  for(const auto& contour : contours) {
    double area = cv::contourArea(contour);
    // ある程度の面積がないとノイズとみなす
    if(area > MIN_LINE_CONTOUR_AREA && area > maxArea) {
      maxArea = area;
      largestContour = contour;
    }
  }

  // ラインが見つからなかった場合、処理を終了
  if(largestContour.empty()) {
    return;
  }

  result.wasDetected = true;

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
}