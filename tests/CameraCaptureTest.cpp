/**
 * @file   CameraCaptureTest.cpp
 * @brief  CameraCaptureクラスのテスト
 * @author Hara1274
 */

#include <gtest/gtest.h>
#include "CameraCapture.h"

namespace etrobocon2025_test {
  // CameraCaptureクラスがインスタンス化できるか確認するテスト:
  // CameraCaptureクラスが正しくインスタンス化され、初期状態のカメラIDが期待通りであることを検証する。
  TEST(CameraCaptureTest, CameraCaptureInit)
  {
    CameraCapture cap;
    int expected = 0;

    EXPECT_EQ(expected, cap.getCameraID());
  }

  // 有効なcameraIDを指定した場合のテスト:
  // 有効なカメラIDを設定した際に、そのIDが正しく反映されることを検証する。
  TEST(CameraCaptureTest, SetCameraIDTrue)
  {
    CameraCapture cap;
    int expected = 1;
    int cameraID = 1;

    ASSERT_TRUE(cap.setCameraID(cameraID));
    EXPECT_EQ(expected, cap.getCameraID());
  }

  // 無効なcameraIDを指定した場合のテスト:
  // 無効なカメラIDを設定した際に、IDが変更されず、エラーが返されることを検証する。
  TEST(CameraCaptureTest, SetCameraIDFalse)
  {
    CameraCapture cap;
    int expected = 0;
    int cameraID = -1;

    ASSERT_FALSE(cap.setCameraID(cameraID));
    EXPECT_EQ(expected, cap.getCameraID());
  }
}  // namespace etrobocon2025_test