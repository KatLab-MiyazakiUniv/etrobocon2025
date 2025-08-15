/**
 * @file DummyBoundingBoxDetector.h
 * @brief 画像処理の親クラス(ダミー)
 * @author HaruArima08 miyahara046
 */

#ifndef DUMMY_BOUNDING_BOX_DETECTOR_H
#define DUMMY_BOUNDING_BOX_DETECTOR_H

#include <random>

class DummyBoundingBoxDetector : public BoundingBoxDetector {
 public:
  /**
   * コンストラクタ
   * @param _useRandomSeed 乱数シードを使用するかどうか
   * @param _seed 乱数シード値
   */
  DummyBoundingBoxDetector(bool _useRandomSeed = false, unsigned int _seed = 0)
    : useRandomSeed(_useRandomSeed), generator(_seed)
  {
  }

  /**
   * @brief 物体検出を実行する純粋仮想関数
   * @param frame 処理対象の画像フレーム
   * @param result 結果を格納するBoundingBoxDetectionResult構造体の参照
   */
  void detect(const cv::Mat&, BoundingBoxDetectionResult& result) override
  {
    if(useRandomSeed) {
      std::uniform_int_distribution<int> dist(0, 1);
      result.wasDetected = (dist(generator) == 1);
    } else {
      // 常に検出成功
      result.wasDetected = true;
    }

    if(result.wasDetected) {
      result.topLeft = cv::Point(100, 100);
      result.topRight = cv::Point(200, 100);
      result.bottomLeft = cv::Point(100, 200);
      result.bottomRight = cv::Point(200, 200);
    }
  }

 private:
  bool useRandomSeed;      // 乱数シードを使用するかどうかのフラグ
  std::mt19937 generator;  // 乱数シード
};

#endif
