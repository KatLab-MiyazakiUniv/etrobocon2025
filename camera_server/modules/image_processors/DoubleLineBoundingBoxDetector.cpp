/**
 * @file DoubleLineBoundingBoxDetector.cpp
 * @brief ライントレース用の画像処理クラス (2色対応)
 * @author takuchi17 miyahara046 HaruArima08
 */
#include "DoubleLineBoundingBoxDetector.h"

// ROIと解像度を指定するオーバーロードコンストラクタ
DoubleLineBoundingBoxDetector::DoubleLineBoundingBoxDetector(
    const cv::Scalar& _lowerHSV1, const cv::Scalar& _upperHSV1, const cv::Scalar& _lowerHSV2,
    const cv::Scalar& _upperHSV2, const cv::Rect& _roi, const cv::Size& _resolution)
  : lowerHSV1(_lowerHSV1),
    upperHSV1(_upperHSV1),
    lowerHSV2(_lowerHSV2),
    upperHSV2(_upperHSV2),
    roi(_roi),
    resolution(_resolution)
{
  validateParameters();
}

// ROIを指定するオーバーロードコンストラクタ (解像度はデフォルト 640x480)
DoubleLineBoundingBoxDetector::DoubleLineBoundingBoxDetector(const cv::Scalar& _lowerHSV1,
                                                             const cv::Scalar& _upperHSV1,
                                                             const cv::Scalar& _lowerHSV2,
                                                             const cv::Scalar& _upperHSV2,
                                                             const cv::Rect& _roi)
  : DoubleLineBoundingBoxDetector(_lowerHSV1, _upperHSV1, _lowerHSV2, _upperHSV2, _roi,
                                  cv::Size(640, 480))
{
}

// デフォルトのROIおよび解像度を使用するコンストラクタ
DoubleLineBoundingBoxDetector::DoubleLineBoundingBoxDetector(const cv::Scalar& _lowerHSV1,
                                                             const cv::Scalar& _upperHSV1,
                                                             const cv::Scalar& _lowerHSV2,
                                                             const cv::Scalar& _upperHSV2)
  : DoubleLineBoundingBoxDetector(_lowerHSV1, _upperHSV1, _lowerHSV2, _upperHSV2,
                                  cv::Rect(50, 240, 540, 240), cv::Size(640, 480))
{
}

void DoubleLineBoundingBoxDetector::validateParameters()
{
  // 解像度の検証
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

  // ROI検証
  if(roi.x < 0) roi.x = 0;
  if(roi.y < 0) roi.y = 0;
  if(roi.x + roi.width > resolution.width) roi.width = resolution.width - roi.x;
  if(roi.y + roi.height > resolution.height) roi.height = resolution.height - roi.y;
}

void DoubleLineBoundingBoxDetector::detect(const cv::Mat& frame, BoundingBoxDetectionResult& result)
{
  result.wasDetected = false;

  if(frame.empty()) {
    std::cerr << "Error: Input frame is empty." << std::endl;
    return;
  }

  // 1. リサイズ処理
  cv::Mat frameProcessed;
  if(frame.size() != resolution) {
    cv::resize(frame, frameProcessed, resolution);
  } else {
    frameProcessed = frame.clone();
  }

  // 2. ROIクリップと切り出し
  cv::Rect roiRect = roi;
  roiRect = roiRect & cv::Rect(0, 0, frameProcessed.cols, frameProcessed.rows);
  if(roiRect.empty()) {
    std::cerr << "Error: ROI is empty after clipping." << std::endl;
    return;
  }
  cv::Mat roiFrame = frameProcessed(roiRect);

  // 3. HSV変換
  cv::Mat hsvFrame;
  cv::cvtColor(roiFrame, hsvFrame, cv::COLOR_BGR2HSV);

  // 4. 2つのHSV範囲でマスクを作成し、結合する
  cv::Mat mask1, mask2;
  // 1色目のマスク
  cv::inRange(hsvFrame, lowerHSV1, upperHSV1, mask1);
  // 2色目のマスク
  cv::inRange(hsvFrame, lowerHSV2, upperHSV2, mask2);

  // 2つのマスクを結合
  cv::Mat combinedMask;
  cv::bitwise_or(mask1, mask2, combinedMask);

  // 5. モルフォロジー処理 (結合されたマスクに対して実行)
  cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));

  // オープニング（ノイズ除去）
  cv::morphologyEx(combinedMask, combinedMask, cv::MORPH_OPEN, kernel);

  // クロージング（ラインの結合 - 隣接する色がここで結合される）
  cv::morphologyEx(combinedMask, combinedMask, cv::MORPH_CLOSE, kernel);

  // 6. 輪郭検出
  std::vector<std::vector<cv::Point>> contours;
  cv::findContours(combinedMask.clone(), contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

  // 7. 最大輪郭の特定
  double maxArea = 0;
  std::vector<cv::Point> largestContour;
  for(const auto& contour : contours) {
    double area = cv::contourArea(contour);
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

  // 8. 外接矩形の計算と座標変換
  cv::Rect boundingBox = cv::boundingRect(largestContour);

  // 座標をROIオフセット分加算して、元のフレーム座標に戻す
  result.topLeft = cv::Point(boundingBox.x + roiRect.x, boundingBox.y + roiRect.y);
  result.topRight
      = cv::Point(boundingBox.x + boundingBox.width + roiRect.x, boundingBox.y + roiRect.y);
  result.bottomLeft
      = cv::Point(boundingBox.x + roiRect.x, boundingBox.y + boundingBox.height + roiRect.y);
  result.bottomRight = cv::Point(boundingBox.x + boundingBox.width + roiRect.x,
                                 boundingBox.y + boundingBox.height + roiRect.y);
}