/**
 * @file   CameraCaptureTest.cpp
 * @brief  CameraCaptureクラスのテスト
 * @author Hara1274
 */

#include <gtest/gtest.h>
#include "CameraCapture.h"

namespace etrobocon2025_test {
  // CameraCaptureクラスがインスタンス化ができるか確認するテスト
  TEST(CameraCaptureTest, CameraCaptureInit)
  {
    CameraCapture cap;
    int expected = 0;

    EXPECT_EQ(expected, cap.getCameraID());
  }

  // 有効なcameraIDを指定した場合のテスト
  TEST(CameraCaptureTest, SetCameraIDTrue)
  {
    CameraCapture cap;
    int expected = 1;
    int cameraID = 1;

    ASSERT_TRUE(cap.setCameraID(cameraID));
    EXPECT_EQ(expected, cap.getCameraID());
  }

  // 無効なcameraIDを指定した場合のテスト
  TEST(CameraCaptureTest, SetCameraIDFalse)
  {
    CameraCapture cap;
    int expected = 0;
    int cameraID = -1;

    ASSERT_FALSE(cap.setCameraID(cameraID));
    EXPECT_EQ(expected, cap.getCameraID());
  }
}  // namespace etrobocon2025_test