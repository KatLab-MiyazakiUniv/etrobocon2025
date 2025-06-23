/**
 * @file DummyBoundingBoxDetector.h
 * @brief 画像処理の親クラス(ダミー)
 * @author HaruArima08
 */

#ifndef DUMMY_BOUNDING_BOX_DETECTOR_H
#define DUMMY_BOUNDING_BOX_DETECTOR_H

class DummyBoundingBoxDetector : public BoundingBoxDetector {
 public:
  /**
   * @brief 物体検出を実行する純粋仮想関数
   * @param frame 処理対象の画像フレーム
   * @param result 結果を格納するBoundingBoxDetectionResult構造体の参照
   */
  void detect(const cv::Mat&, BoundingBoxDetectionResult& result) override
  {
    result.wasDetected = true;
    result.topLeft = cv::Point(100, 100);
    result.topRight = cv::Point(200, 100);
    result.bottomLeft = cv::Point(100, 200);
    result.bottomRight = cv::Point(200, 200);
  }
};

#endif
