/**
 * @file   ImageUploaderTest.cpp
 * @brief  ImageUploaderクラスをテストする
 * @author Hara1274 takuchi17
 */

#include "ImageUploader.h"
#include "dummy/DummyCameraCapture.h"
#include "FrameSave.h"
#include <gtest/gtest.h>
#include <opencv2/opencv.hpp>

namespace etrobocon2025_test {
  // 空文字列のファイルパスで失敗することをテスト
  TEST(ImageUploaderTest, EmptyFilePathShouldFail)
  {
    bool result = ImageUploader::uploadImage("", 1);
    EXPECT_FALSE(result);
  }

  // ゼロ回試行で失敗することをテスト
  TEST(ImageUploaderTest, ZeroRetriesShouldFail)
  {
    bool result = ImageUploader::uploadImage("test.jpg", 0);
    EXPECT_FALSE(result);
  }

  // 負の試行回数で失敗することをテスト
  TEST(ImageUploaderTest, NegativeRetriesShouldFail)
  {
    bool result = ImageUploader::uploadImage("test.jpg", -1);
    EXPECT_FALSE(result);
  }

  // 存在しないファイルで失敗することをテスト
  TEST(ImageUploaderTest, NonExistentFileShouldFail)
  {
    bool result = ImageUploader::uploadImage("nonexistent_file.jpg", 1);
    EXPECT_FALSE(result);
  }

  // 実在するファイルでアップロード処理が実行されることをテスト
  TEST(ImageUploaderTest, ValidFileUploadReturnsTrue)
  {
    // 画像保存の準備
    DummyCameraCapture camera;
    cv::Mat frame;
    camera.getFrame(frame);

    std::string filePath = "../../../tests/test_images/";
    std::string fileName = "test_data";

    FrameSave::save(frame, filePath, fileName);

    std::string validPath = filePath + fileName;

    // CommandExecutor::exec は dummy で常に0を返すので
    // uploadImageは成功するはず
    bool result = ImageUploader::uploadImage(validPath, 1);

    // テスト用画像ファイルを削除
    std::string filePathWithExtension = validPath + ".JPEG";
    std::remove(filePathWithExtension.c_str());

    // 実際に true を返すかをチェック
    EXPECT_TRUE(result);
  }
}  // namespace etrobocon2025_test