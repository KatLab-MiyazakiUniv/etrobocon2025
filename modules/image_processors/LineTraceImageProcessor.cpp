#include "LineTraceImageProcessor.h"

LineTraceImageProcessor::LineTraceImageProcessor(const cv::Scalar& _lowerHSV,
                                                 const cv::Scalar& _upperHSV)
  : lowerHSV(_lowerHSV), upperHSV(_upperHSV)
{
}

DetectionResult LineTraceImageProcessor::process(const cv::Mat& frame)
{
  /**
   * @todo: ライントレース用の画像処理を実装する
   */
}
