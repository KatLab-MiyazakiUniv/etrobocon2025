/**
 * @file   DualLineBoundingBoxDetector.h
 * @brief  色切り替えライントレース用の画像処理クラス
 * @author nishijima515
 */

#ifndef DUAL_LINE_BOUNDING_BOX_DETECTOR_H
#define DUAL_LINE_BOUNDING_BOX_DETECTOR_H

#include "BoundingBoxDetector.h"
#include "SystemInfo.h"
#include "Robot.h"
#include "Snapshot.h"
#include <iostream>

class DualLineBoundingBoxDetector : public BoundingBoxDetector {
 public:
  /**
   * @brief デフォルトのROIおよび解像度を使用するコンストラクタ
   * @param lowerHSV ライントレース対象の色の下限HSV値(1色目)
   * @param upperHSV ライントレース対象の色の上限HSV値(1色目)
   * @param lowerHSV2 ライントレース対象の色の下限HSV値(2色目)
   * @param upperHSV2 ライントレース対象の色の上限HSV値(2色目)
   * @param switchAreaThreshold 色切り替えのための面積閾値
   */
  DualLineBoundingBoxDetector(const cv::Scalar& _lowerHSV, const cv::Scalar& _upperHSV,
                              const cv::Scalar& _lowerHSV2, const cv::Scalar& _upperHSV2,
                              const double switchAreaThreshold);

  /**
   * @brief ROIを指定するオーバーロードコンストラクタ
   * @param lowerHSV ライントレース対象の色の下限HSV値
   * @param upperHSV ライントレース対象の色の上限HSV値
   * @param lowerHSV2 ライントレース対象の色の下限HSV値(2色目)
   * @param upperHSV2 ライントレース対象の色の上限HSV値(2色目)
   * @param switchAreaThreshold 色切り替えのための面積閾値
   * @param roi 注目領域
   */
  DualLineBoundingBoxDetector(const cv::Scalar& _lowerHSV, const cv::Scalar& _upperHSV,
                              const cv::Scalar& _lowerHSV2, const cv::Scalar& _upperHSV2,
                              const double switchAreaThreshold, const cv::Rect& _roi);

  /**
   * @brief ROIと解像度を指定するオーバーロードコンストラクタ
   * @param lowerHSV ライントレース対象の色の下限HSV値
   * @param upperHSV ライントレース対象の色の上限HSV値
   * @param lowerHSV2 ライントレース対象の色の下限HSV値(2色目)
   * @param upperHSV2 ライントレース対象の色の上限HSV値(2色目)
   * @param switchAreaThreshold 色切り替えのための面積閾値
   * @param roi 注目領域
   * @param resolution 画像処理に用いる解像度
   */
  DualLineBoundingBoxDetector(const cv::Scalar& _lowerHSV, const cv::Scalar& _upperHSV,
                              const cv::Scalar& _lowerHSV2, const cv::Scalar& _upperHSV2,
                              const double switchAreaThreshold, const cv::Rect& _roi,
                              const cv::Size& _resolution);

  /**
   * @brief 画像処理を実行する関数
   * @param frame 処理対象の画像フレーム
   * @param result 結果を格納するBoundingBoxDetectionResult構造体の参照
   */
  void detect(const cv::Mat& frame, BoundingBoxDetectionResult& result) override;

  /**
   * @brief 前回の検出から色が切り替わったかどうかを取得する
   * @return true: 色が切り替わった, false: 色が切り替わっていない
   */
  bool getWasColorSwitched() const;

 private:
  // 対象の輪郭とみなす最小面積（この閾値は調整）
  static constexpr double MIN_LINE_CONTOUR_AREA = 50.0;
  cv::Scalar lowerHSV;                // ライントレース対象の色の下限HSV値
  cv::Scalar upperHSV;                // ライントレース対象の色の上限HSV値
  cv::Scalar lowerHSV2;               // ライントレース対象の色の下限HSV値(2色目)
  cv::Scalar upperHSV2;               // ライントレース対象の色の上限HSV値(2色目)
  double switchAreaThreshold = 50.0;  // 色切り替えのための面積閾値
  cv::Rect roi;                       // 注目領域
  cv::Size resolution;                // 解像度
  int currentTargetColor = 1;         // 現在の追跡対象の色 (1または2)
  bool wasColorSwitched = false;      // 前回の検出から色が切り替わったかどうか

  /**
   * @brief 入力で受け取ったROIと解像度の検証を行う関数
   */
  void validateParameters();

  /**
   * @brief 指定した色で最大のバウンディングボックスを検出
   * @param hsvFrame HSV変換済みのROI画像
   * @param lowerHSV 下限
   * @param upperHSV 上限
   * @param[out] maxArea 最大面積
   * @return 検出された矩形（無ければ空）
   */
  cv::Rect detectColorBox(const cv::Mat& hsvFrame, const cv::Scalar& lowerHSV,
                          const cv::Scalar& upperHSV, double& maxArea) const;

  /**
   * @brief 前回の検出から色が切り替わったかどうかを設定する
   * @param switched true: 色が切り替わった, false: 色が切り替わっていない
   */
  void setWasColorSwitched(bool switched);
};

#endif
