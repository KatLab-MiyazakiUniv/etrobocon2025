/**
 * @file   LineBoundingBoxDetector.h
 * @brief  ライントレース用の画像処理クラス
 * @author takuchi17 miyahara046 HaruArima08
 */
#include "LineBoundingBoxDetector.h"

// デフォルトコンストラクタ
LineBoundingBoxDetector::LineBoundingBoxDetector(const cv::Scalar& _lowerHSV,
                                                 const cv::Scalar& _upperHSV)
  : lowerHSV(_lowerHSV), upperHSV(_upperHSV)
{
  roi = cv::Rect(50, 240, 540, 240);
  resolution = cv::Size(640, 480);
  validateParameters();
}

// ROIを指定するオーバーロードコンストラクタ
LineBoundingBoxDetector::LineBoundingBoxDetector(const cv::Scalar& lowerHSV,
                                                 const cv::Scalar& upperHSV, const cv::Rect& roi)
  : lowerHSV(lowerHSV), upperHSV(upperHSV), roi(roi)
{
  resolution = cv::Size(640, 480);
  validateParameters();
}

// ROIと解像度を指定するオーバーロードコンストラクタ
LineBoundingBoxDetector::LineBoundingBoxDetector(const cv::Scalar& _lowerHSV,
                                                 const cv::Scalar& _upperHSV, const cv::Rect& _roi,
                                                 const cv::Size& _resolution)
  : lowerHSV(_lowerHSV), upperHSV(_upperHSV), roi(_roi), resolution(_resolution)
{
  validateParameters();
}

void LineBoundingBoxDetector::validateParameters()
{
  // 解像度の検証：設定された解像度が定義した最小・最大値内にあるかの確認
  if(resolution.width < ResolutionRange::MIN_WIDTH) {
    resolution.width = ResolutionRange::MIN_WIDTH;
  } else if(resolution.width > ResolutionRange::MAX_WIDTH) {
    resolution.width = ResolutionRange::MAX_WIDTH;
  }
  if(resolution.height < ResolutionRange::MIN_HEIGHT) {
    resolution.height = ResolutionRange::MIN_HEIGHT;
  } else if(resolution.height > ResolutionRange::MAX_HEIGHT) {
    resolution.height = ResolutionRange::MAX_HEIGHT;
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

  // ROIが空でないか、または画像範囲外でないかのチェック
  if(frame.empty()) {
    std::cerr << "Error: Input frame is empty." << std::endl;
    return;
  }

  // 入力画像が指定の解像度と異なる場合、リサイズ処理
  cv::Mat frameProcessed;
  if(frame.size() != resolution) {
    cv::resize(frame, frameProcessed, resolution);
  } else {
    frameProcessed = frame;
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
  // サイズは調整が必要。
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
  bool lineFound = false;  // ラインが見つかったかどうかのフラグ

  // 輪郭をイテレート
  for(const auto& contour : contours) {
    double area = cv::contourArea(contour);
    // ある程度の面積がないとノイズとみなす（この閾値は調整）
    if(area > MIN_LINE_CONTOUR_AREA) {
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
}