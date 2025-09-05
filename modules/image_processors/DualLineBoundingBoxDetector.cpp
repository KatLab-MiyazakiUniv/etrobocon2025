/**
 * @file   DualLineBoundingBoxDetector.h
 * @brief  色切り替えライントレース用の画像処理クラス
 * @author nishijima515
 */
#include "DualLineBoundingBoxDetector.h"

// ROIと解像度を指定するオーバーロードコンストラクタ
DualLineBoundingBoxDetector::DualLineBoundingBoxDetector(
    const cv::Scalar& _lowerHSV, const cv::Scalar& _upperHSV, const cv::Scalar& _lowerHSV2,
    const cv::Scalar& _upperHSV2, const double _switchAreaThreshold, const cv::Rect& _roi,
    const cv::Size& _resolution)
  : lowerHSV(_lowerHSV),
    upperHSV(_upperHSV),
    lowerHSV2(_lowerHSV2),
    upperHSV2(_upperHSV2),
    switchAreaThreshold(_switchAreaThreshold),
    roi(_roi),
    resolution(_resolution)
{
  validateParameters();
}

// ROIを指定するオーバーロードコンストラクタ
DualLineBoundingBoxDetector::DualLineBoundingBoxDetector(
    const cv::Scalar& _lowerHSV, const cv::Scalar& _upperHSV, const cv::Scalar& _lowerHSV2,
    const cv::Scalar& _upperHSV2, const double _switchAreaThreshold, const cv::Rect& _roi)
  : DualLineBoundingBoxDetector(_lowerHSV, _upperHSV, _lowerHSV2, _upperHSV2, _switchAreaThreshold,
                                _roi, cv::Size(640, 480))
{
}

// デフォルトのROIおよび解像度を使用するコンストラクタ
DualLineBoundingBoxDetector::DualLineBoundingBoxDetector(const cv::Scalar& _lowerHSV,
                                                         const cv::Scalar& _upperHSV,
                                                         const cv::Scalar& _lowerHSV2,
                                                         const cv::Scalar& _upperHSV2,
                                                         const double _switchAreaThreshold)
  : DualLineBoundingBoxDetector(_lowerHSV, _upperHSV, _lowerHSV2, _upperHSV2, _switchAreaThreshold,
                                cv::Rect(50, 240, 540, 240), cv::Size(640, 480))
{
}

void DualLineBoundingBoxDetector::validateParameters()
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

void DualLineBoundingBoxDetector::setWasColorSwitched(bool switched)
{
  wasColorSwitched = switched;
}

bool DualLineBoundingBoxDetector::getWasColorSwitched() const
{
  return wasColorSwitched;
}

cv::Rect DualLineBoundingBoxDetector::detectColorBox(const cv::Mat& hsvFrame,
                                                     const cv::Scalar& lowerHSV,
                                                     const cv::Scalar& upperHSV,
                                                     double& maxArea) const
{
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
  cv::findContours(mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

  // 最大面積を持つ輪郭を特定
  maxArea = 0;
  cv::Rect maxRect;

  // 輪郭をイテレート
  for(const auto& contour : contours) {
    double area = cv::contourArea(contour);
    // ある程度の面積がないとノイズとみなす
    if(area > MIN_LINE_CONTOUR_AREA && area > maxArea) {
      maxArea = area;
      maxRect = cv::boundingRect(contour);
    }
  }
  return maxRect;
}

void DualLineBoundingBoxDetector::detect(const cv::Mat& frame, BoundingBoxDetectionResult& result)
{
  result.wasDetected = false;  // 初期状態は検出失敗

  if(frame.empty()) {
    std::cerr << "Error: Input frame is empty." << std::endl;
    return;
  }

  // 入力画像が指定の解像度と異なる場合、リサイズ処理
  cv::Mat frameProcessed;
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

  double area1 = 0.0;
  double area2 = 0.0;
  cv::Rect box1 = detectColorBox(hsvFrame, lowerHSV, upperHSV, area1);
  cv::Rect box2 = detectColorBox(hsvFrame, lowerHSV2, upperHSV2, area2);

  // 切り替え判定
  if(currentTargetColor == 1 && area2 >= switchAreaThreshold) {
    currentTargetColor = 2;
    setWasColorSwitched(true);
  }
  cv::Rect targetBox = (currentTargetColor == 1) ? box1 : box2;
  if(targetBox.area() > 0) {
    result.wasDetected = true;
    result.topLeft = targetBox.tl() + roiRect.tl();
    result.bottomRight = targetBox.br() + roiRect.tl();
    result.topRight = cv::Point(result.bottomRight.x, result.topLeft.y);
    result.bottomLeft = cv::Point(result.topLeft.x, result.bottomRight.y);
  }
}