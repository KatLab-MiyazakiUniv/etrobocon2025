/**
 * @file   AreaMaster.cpp
 * @brief  エリアを攻略するクラス
 * @author Hara1274
 */

#include "AreaMaster.h"

using namespace std;
const string AreaMaster::basePath = "datafiles/commands/";
const array<string, 3> AreaMaster::areaCommandNames = { "LineTrace", "DoubleLoop", "SmartCarry" };

AreaMaster::AreaMaster(Robot& _robot, Area _area, bool _isLeftCourse, int _targetBrightness)
  : robot(_robot), area(_area), isLeftCourse(_isLeftCourse), targetBrightness(_targetBrightness)
{
}

void AreaMaster::run()
{
  // ファイルから受け取る動作リスト
  vector<Motion*> motionList;

  // コマンドファイルパスを作成する
  string commandFilePath = basePath + areaCommandNames[static_cast<int>(area)]
                           + (isLeftCourse ? "Left" : "Right") + ".csv";

  // 動作インスタンスのリストを生成する
  motionList = MotionParser::createMotions(robot, commandFilePath, targetBrightness);

  // 各動作を実行し、動作し終えたらメモリを開放する
  for(auto motion = motionList.begin(); motion != motionList.end();) {
    (*motion)->run();
    delete *motion;                     // メモリを解放
    motion = motionList.erase(motion);  // リストから削除
  }
}