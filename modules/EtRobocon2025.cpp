/**
 * @file   EtRobocon2025.cpp
 * @brief  全体を制御するクラス
 * @author takahashitom takuchi17
 */

#include "EtRobocon2025.h"
#include "AreaMaster.h"

Robot EtRobocon2025::robot;  // Robotインスタンス

void EtRobocon2025::start()
{
  std::cout << "Hello KATLAB" << std::endl;
  spikeapi::ColorSensor::HSV hsv;
  spikeapi::ColorSensor& colorSensor = robot.getColorSensorInstance();
  while(1) {
    colorSensor.getHSV(hsv);
    std::cout << "h: " << hsv.h << ", s: " << (int)hsv.s << ", v: " << (int)hsv.v << std::endl;
    COLOR color = ColorJudge::convertHsvToColor(hsv);
    std::cout << "color: " << ColorJudge::convertColorToString(color) << std::endl;
    tslp_tsk(1000000);
  }
}