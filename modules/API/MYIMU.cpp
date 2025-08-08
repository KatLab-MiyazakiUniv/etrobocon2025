#include "MYIMU.h"
#include <math.h>

MYIMU::MYIMU() {
    static float offset[3] = {-1.61239f, -1.485107f, -0.2945677f};
    static float scale[3]  = {360.4545f, 356.9208f, 363.781f};
    static float calib[6]  = {10016.18f, -9657.935f, 9823.967f, -9957.187f, 9766.231f, -9970.058f};

    hub_imu_initialize(2.0f, 2500.0f, offset, scale, calib);
}



float MYIMU::getHeading() const {
  return hub_imu_get_heading();
}