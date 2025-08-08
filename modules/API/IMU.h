// IMU.h(自作する。公式ラッパーにはgetHeadingがないかつこれしか使わないため。)
#ifndef IMU_H
#define IMU_H
extern "C" {
#include <spike/hub/imu.h>
#include <raspike_additional_api.h>
}

class IMU {
 public:
  IMU() { hub_imu_init(); }

  // ヘディング取得（累積）
  float getHeading() const { return hub_imu_get_heading(); }
};

#endif  // IMU_H
