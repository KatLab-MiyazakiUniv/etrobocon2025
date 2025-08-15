/**
 * @file MiniFigDirectionDetectorTest.cpp
 * @brief ミニフィグ向き判定クラスのテスト
 * @author nishijima515
 */

// モデルが必要なため、コメントアウト

// #include <gtest/gtest.h>
// #include "MiniFigDirectionDetector.h"
// #include <unistd.h>

// TEST(MiniFigDirectionDetectorTest, DetectsDirectionFromImage)
// {
//   char cwd[1024];
//   getcwd(cwd, sizeof(cwd));
//   std::cout << "Current working directory: " << cwd << std::endl;
//   MiniFigDirectionDetector detector("../../datafiles/models/yolo_optimized.onnx");

//   // 入力画像を読み込み
//   cv::Mat img = cv::imread("../../../tests/test_images/Fig1-1.jpeg");
//   ASSERT_FALSE(img.empty()) << "画像の読み込みに失敗しました";

//   MiniFigDirectionResult result;
//   detector.detect(img, result);

//   ASSERT_TRUE(result.wasDetected);
//   EXPECT_EQ(result.direction, MiniFigDirection::FRONT);
// }