/**
 * @file   AreaMaster.cpp
 * @brief  エリアを攻略するクラス
 * @author Hara1274
 */

#include "AreaMaster.h"
#include <fstream>
#include <iostream>

using namespace std;
const string AreaMaster::basePath = "etrobocon2025/datafiles/commands/";
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
  executeMotions(motionList);
}

void AreaMaster::runConditional()
{
  // 条件分岐ファイルの存在確認
  if(!hasConditionalFiles()) {
    cout << "ミニフィグ判定結果に基づく分岐ファイルが見つからないため、通常のエリア走行を実行します"
         << endl;
    run();
    return;
  }

  cout << "条件分岐版でエリアを実行します。" << endl;

  // 分岐前の共通動作実行
  string commonFile = getConditionalFilePath("_Common");
  cout << "分岐前共通動作ファイル: " << commonFile << endl;
  vector<Motion*> commonMotions = MotionParser::createMotions(robot, commonFile, targetBrightness);
  executeMotions(commonMotions);

  // ミニフィグ判定結果に基づく分岐
  MiniFigDirectionResult& result = robot.getMiniFigDirectionResult();

  string nextFile;
  if(result.wasDetected) {
    cout << "ミニフィグ判定成功 - FigJudgeSuccess動作を実行" << endl;
    nextFile = getConditionalFilePath("_FigJudgeSuccess");
  } else {
    cout << "ミニフィグ判定失敗 - FigJudgeFailure動作を実行" << endl;
    nextFile = getConditionalFilePath("_FigJudgeFailure");
  }

  cout << "条件分岐後動作ファイル: " << nextFile << endl;
  vector<Motion*> conditionalMotions
      = MotionParser::createMotions(robot, nextFile, targetBrightness);
  executeMotions(conditionalMotions);
}

void AreaMaster::executeMotions(vector<Motion*>& motionList)
{
  for(auto motion = motionList.begin(); motion != motionList.end();) {
    (*motion)->run();
    delete *motion;                     // メモリを解放
    motion = motionList.erase(motion);  // リストから削除
  }
}

bool AreaMaster::hasConditionalFiles()
{
  string commonFile = getConditionalFilePath("_Common");
  ifstream file(commonFile);
  return file.good();
}

string AreaMaster::getConditionalFilePath(const string& suffix)
{
  return basePath + areaCommandNames[static_cast<int>(area)] + (isLeftCourse ? "Left" : "Right")
         + suffix + ".csv";
}