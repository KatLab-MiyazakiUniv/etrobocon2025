/**
 * @file MiniFigDirectionDetectorTest.cpp
 * @brief ミニフィグ向き判定クラスのテスト
 * @author nishijima515
 */

// モデルが必要なため、コメントアウト

// #include <gtest/gtest.h>
// #include "MiniFigDirectionDetector.h"

// TEST(MiniFigDirectionDetectorTest, DetectsDirectionFromImage)
// {
//   MiniFigDirectionDetector detector("../../yolo_optimized.onnx");

//   // 入力画像を読み込み
//   cv::Mat img = cv::imread("../../tests/test_images/Fig1-3.jpeg");
//   ASSERT_FALSE(img.empty()) << "画像の読み込みに失敗しました";

//   MiniFigDirectionResult result;
//   detector.detect(img, "../../datafiles/snapshots/detected_result.jpg", result);

//   ASSERT_TRUE(result.wasDetected);
//   EXPECT_EQ(result.direction, MiniFigDirection::BACK);
// }