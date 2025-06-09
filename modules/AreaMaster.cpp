/**
 * @file   AreaMaster.cpp
 * @brief  エリアを攻略するクラス
 * @author Hara1274
 */

#include "AreaMaster.h"
#include <typeinfo>

namespace fs = std::filesystem;
using namespace std;

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

  // 各動作を実行する
  for(auto motion = motionList.begin(); motion != motionList.end();) {
    (*motion)->run();
    delete *motion;                     // メモリを解放
    motion = motionList.erase(motion);  // リストから削除
  }
}