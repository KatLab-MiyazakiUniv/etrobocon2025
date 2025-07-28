/**
 * @file   ImageUploaderTest.cpp
 * @brief  ImageUploaderクラスをテストする
 * @author Hara1274
 */

#include "ImageUploader.h"
#include <gtest/gtest.h>

namespace etrobocon2025_test {
  // 空文字列のファイルパスで失敗することをテスト
  TEST(ImageUploaderTest, EmptyFilePathShouldFail)
  {
    ImageUploader uploader;
    bool result = uploader.uploadImage("", 1);
    EXPECT_FALSE(result);
  }

  // ゼロ回試行で失敗することをテスト
  TEST(ImageUploaderTest, ZeroRetriesShouldFail)
  {
    ImageUploader uploader;
    bool result = uploader.uploadImage("test.jpg", 0);
    EXPECT_FALSE(result);
  }

  // 負の試行回数で失敗することをテスト
  TEST(ImageUploaderTest, NegativeRetriesShouldFail)
  {
    ImageUploader uploader;
    bool result = uploader.uploadImage("test.jpg", -1);
    EXPECT_FALSE(result);
  }

  // 存在しないファイルで失敗することをテスト
  TEST(ImageUploaderTest, NonExistentFileShouldFail)
  {
    ImageUploader uploader;
    bool result = uploader.uploadImage("nonexistent_file.jpg", 1);
    EXPECT_FALSE(result);
  }

  // コンストラクタが正常に動作することをテスト
  TEST(ImageUploaderTest, Constructor)
  {
    EXPECT_NO_THROW(ImageUploader uploader);
  }

  // 実在するファイルでアップロード処理が実行されることをテスト
  TEST(ImageUploaderTest, ValidFileUpload)
  {
    ImageUploader uploader;
    std::string validPath = "../../tests/test_images/test_data.JPEG";
    // サーバーの状態に関係なく例外が発生しないことを確認
    EXPECT_NO_THROW(uploader.uploadImage(validPath, 1));
  }
}  // namespace etrobocon2025_test