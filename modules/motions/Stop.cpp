/**
 * @file   Stop.cpp
 * @brief  走行体を停止させる動作
 * @author Hara1274 HaruArima08
 */

#include "Stop.h"
#include "MotorController.h"

Stop::Stop(Robot& _robot) : Motion(_robot) {}

void Stop::run()
{
  robot.getMotorControllerInstance().stopWheelsMotor();
}