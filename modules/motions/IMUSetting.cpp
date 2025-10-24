/**
 * @file   IMUSetting.cpp
 * @brief  IMU角度の測定を設定する動作
 * @author hara1274
 */

#include "IMUSetting.h"

IMUSetting::IMUSetting(Robot& _robot, bool _setting) : Motion(_robot), setting(_setting) {}

void IMUSetting::run()
{
  if(setting == true) {
    robot.getIMUControllerInstance().resetAngle();
    robot.getIMUControllerInstance().startAngleCalculation();
    robot.getIMUControllerInstance().setShouldContinueCalculation(true);
  } else {
    robot.getIMUControllerInstance().stopAngleCalculation();
    robot.getIMUControllerInstance().setShouldContinueCalculation(false);
  }
}
