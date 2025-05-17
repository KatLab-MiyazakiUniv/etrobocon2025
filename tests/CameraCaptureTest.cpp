/**
 * @file   CameraControllerTest.cpp
 * @brief  Cameraクラスのテスト
 * @author HaraNaruki
 */

#include <gtest/gtest.h>
#include "../usb_camera/CameraCapure.h"
#include "helpers/OStreamCapture.h"

using namespace std;

namespace etrobocon2025_test {
  // CameraCaptureクラスがインスタンス化ができるか確認するテスト
  TEST(CameraCapureTest, CameraCapureInit)
  {
    CameraCapture cap;
    int expected = 0;

    EXPECT_EQ(expected, cap.getCameraID());
  }

  // 有効なcameraIDを指定した場合のテスト
  TEST(CameraCapureTest, SetCameraIDTrue)
  {
    CameraCapture cap;
    int expected = 1;
    int cameraID = 1;

    ASSERT_TRUE(cap.setCameraID(cameraID));
    EXPECT_EQ(expected, cap.getCameraID());
  }

  // 無効なcameraIDを指定した場合のテスト
  TEST(CameraCapureTest, SetCameraIDFalse)
  {
    CameraCapture cap;
    int expected = 0;
    int cameraID = -1;

    ASSERT_FALSE(cap.setCameraID(cameraID));
    EXPECT_EQ(expected, cap.getCameraID());
  }
}  // namespace etrobocon2025_test