/**
 * @file BackgroundDirectionDetectorTest.cpp
 * @brief 風景向き判定クラスのテスト
 * @author Hara1274
 */

// #include <gtest/gtest.h>
// #include "BackgroundDirectionDetector.h"

// namespace etrobocon2025_test {

//   // テスト用の画像を読み込めるかのテスト
//   TEST(BackgroundDirectionDetectorTest, DetectsBackgroundDirectionFromImage)
//   {
//     BackgroundDirectionDetector detector("../../datafiles/models/BackgroundDetector.onnx");

//     // 入力画像を読み込み
//     cv::Mat img = cv::imread("../../tests/test_images/Background_Back_image.JPEG");
//     ASSERT_FALSE(img.empty()) << "画像の読み込みに失敗しました";

//     BackgroundDirectionResult result;
//     detector.detect(img, result);

//     ASSERT_TRUE(result.wasDetected);
//     EXPECT_EQ(result.direction, BackgroundDirection ::FRONT);
//   }
// }  // namespace etrobocon2025_test