/**
 * @file   EtRobocon2025.cpp
 * @brief  全体を制御するクラス
 * @author takahashitom takuchi17
 */

#include "EtRobocon2025.h"
#include "MotionParser.h"

Robot EtRobocon2025::robot;  // Robotインスタンス

void EtRobocon2025::start()
{
  std::cout << "Hello KATLAB" << std::endl;
  std::string commandFilePath = "etrobocon2025/datafiles/commands/DistanceCameraLineTrace.csv";

  if(!robot.getCameraCaptureInstance().setCameraID(robot.getCameraCaptureInstance().findAvailableCameraID())) return;
  if(!robot.getCameraCaptureInstance().openCamera()) return;
  robot.getCameraCaptureInstance().setCapProps(640,480);

robot.getClockInstance().sleep(10000);

  int targetBrightness = 45;

  std::vector<Motion*> motions = MotionParser::createMotions(
    robot, commandFilePath, targetBrightness);
    for(size_t i = 0; i < motions.size(); i++) {
      if(motions[i]!=nullptr){
        motions[i]->run();
        std::cout <<(i+1) << " -- " << "動作完了" << std::endl;
      }
    }
  for(Motion*motion : motions) {
    delete motion;
  }
}