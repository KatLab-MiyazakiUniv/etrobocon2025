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
    std::string validPath = "../../tests/test_images/test_data.JPEG";
    // サーバーの状態に関係なく例外が発生しないことを確認
    EXPECT_NO_THROW(ImageUploader::uploadImage(validPath, 1));
  }
}  // namespace etrobocon2025_test