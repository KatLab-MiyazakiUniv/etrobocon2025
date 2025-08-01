/**
 * @file   ImageUploaderTest.cpp
 * @brief  ImageUploaderクラスをテストする
 * @author Hara1274
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
  TEST(ImageUploaderTest, ValidFileUpload)
  {
    // DummyCameraCaptureを使って実際の画像を作成
    DummyCameraCapture camera;
    cv::Mat frame;
    camera.getFrame(frame);

    // FrameSaveを使って画像を保存
    std::string filePath = "../../tests/test_images/";
    std::string fileName = "test_data";
    FrameSave::save(frame, filePath, fileName);

    std::string validPath = filePath + fileName;
    bool result;
    // サーバーの状態に関係なく例外が発生しないことを確認
    EXPECT_NO_THROW({ result = ImageUploader::uploadImage(validPath, 1); });

    // テストファイルを削除
    std::string filePathWithExtension = filePath + fileName + ".JPEG";
    std::remove(filePathWithExtension.c_str());

    // サーバーがないとmakeコマンドがエラーを返すのでコメントアウト
    // EXPECT_TRUE(result);
  }
}  // namespace etrobocon2025_test