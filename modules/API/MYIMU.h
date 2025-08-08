// MYIMU.h(自作する。公式ラッパーにはgetHeadingがないかつこれしか使わないため。)
#ifndef IMU_H
#define IMU_H
extern "C" {
#include <spike/hub/imu.h>
#include <raspike_additional_api.h>
}

class MYIMU {
 public:
  MYIMU();

  // ヘディング取得（累積）
  float getHeading() const;
};

#endif  // IMU_H
