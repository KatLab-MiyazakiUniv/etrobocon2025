/**
 * @file Calibrator.cpp
 * @brief キャリブレーションからスタートまでを担当するクラス
 * @author HaruArima08 nishijima515
 */

#include "Calibrator.h"

using namespace std;

Calibrator::Calibrator(Robot& _robot)
  : robot(_robot), isLeftCourse(true), isLeftEdge(true), targetBrightness(50)
{
}

void Calibrator::run()
{
  // 左右ボタンでコースのLRを選択する
  selectAndSetCourse();

  // 目標輝度を測定する
  measureAndSetTargetBrightness();
}

void Calibrator::selectAndSetCourse()
{
  const char* text1 = "L OR R";
  robot.getDisplayInstance().scrollText(text1, 50);
  // 右ボタンが押されたら確定する
  while(!robot.getButtonInstance().isRightPressed()) {
    if(robot.getButtonInstance().isLeftPressed() && !isLeftCourse) {
      // 左ボタンが押されたときRコースがセットされていれば、Lコースをセットする
      isLeftCourse = true;
      isLeftEdge = true;
      robot.setIsLeftEdge(isLeftEdge);  // Lコースを選択したので、左エッジとする
      // 画面にLコースが選択されたことを表示
      robot.getDisplayInstance().showChar(LEFT);
      robot.getClockInstance().sleep(10000);  // 10ミリ秒スリープ

      // // ボタンが離されるまで待機
      // while(robot.getButtonInstance().isLeftPressed()) {
      //   robot.getClockInstance().sleep(10000);
      // }

    } else if(robot.getButtonInstance().isLeftPressed() && isLeftCourse) {
      // 左ボタンが押されたときLコースがセットされていれば、Rコースをセットする
      isLeftCourse = false;
      isLeftEdge = false;
      robot.setIsLeftEdge(isLeftEdge);  // Rコースを選択したので、右エッジとする
      // 画面にRコースが選択されたことを表示
      robot.getDisplayInstance().showChar(RIGHT);
      robot.getClockInstance().sleep(10000);  // 10ミリ秒スリープ

      // // ボタンが離されるまで待機
      // while(robot.getButtonInstance().isLeftPressed()) {
      //   robot.getClockInstance().sleep(10000);
      // }
    }
  }
  const char* course = isLeftCourse ? "Left" : "Right";
  printf("\nWill Run on the %s Course\n", course);
  const char* text2 = "OK";
  robot.getDisplayInstance().scrollText(text2, 50);
  robot.getClockInstance().sleep(2000000);  // 2秒スリープ
}

void Calibrator::measureAndSetTargetBrightness()
{
  int blackBrightness = -1;
  int whiteBrightness = -1;

  const char* text1 = "B";
  robot.getDisplayInstance().scrollText(text1, 50);

  // 黒の輝度測定
  // 左ボタンで輝度を取得し、右ボタンで黒の輝度を決定する
  bool isLoopStarted = false;
  while(1) {
    // 左ボタンが押されたら連続取得ループを開始
    if(robot.getButtonInstance().isLeftPressed() && !isLoopStarted) {
      isLoopStarted = true;
      // // ボタンが離されるまで待機
      // while(robot.getButtonInstance().isLeftPressed()) {
      //   robot.getClockInstance().sleep(10000);  // 10ミリ秒スリープ
      // }
    }

    // ループが開始されたら連続的に輝度を取得
    if(isLoopStarted) {
      blackBrightness = robot.getColorSensorInstance().getReflection();
      // 現在の輝度値をリアルタイムで表示
      robot.getDisplayInstance().showNumber(blackBrightness);

      // 右ボタンが押されたら確定
      if(robot.getButtonInstance().isRightPressed()) {
        // 黒の輝度を確定したことをディスプレイに表示
        const char* text2 = "OK";
        robot.getDisplayInstance().scrollText(text2, 50);
        robot.getClockInstance().sleep(2000000);  // 2秒スリープ

        // // ボタンが離されるまで待機
        // while(robot.getButtonInstance().isRightPressed()) {
        //   robot.getClockInstance().sleep(10000);  // 10ミリ秒スリープ
        // }
        break;
      }
    }

    robot.getClockInstance().sleep(10000);  // 10ミリ秒スリープ
  }

  const char* text3 = "W";
  robot.getDisplayInstance().scrollText(text3, 50);

  // 白の輝度測定
  // 左ボタンで輝度を取得し、右ボタンで白の輝度を決定する
  isLoopStarted = false;
  while(1) {
    // 左ボタンが押されたら連続取得ループを開始
    if(robot.getButtonInstance().isLeftPressed() && !isLoopStarted) {
      isLoopStarted = true;
      // // ボタンが離されるまで待機
      // while(robot.getButtonInstance().isLeftPressed()) {
      //   robot.getClockInstance().sleep(10000);  // 10ミリ秒スリープ
      // }
    }

    // ループが開始されたら連続的に輝度を取得
    if(isLoopStarted) {
      whiteBrightness = robot.getColorSensorInstance().getReflection();
      // 現在の輝度値をリアルタイムで表示
      robot.getDisplayInstance().showNumber(whiteBrightness);

      // 右ボタンが押されたら確定
      if(robot.getButtonInstance().isRightPressed()) {
        // 黒の輝度を確定したことをディスプレイに表示
        const char* text2 = "OK";
        robot.getDisplayInstance().scrollText(text2, 50);
        robot.getClockInstance().sleep(2000000);  // 2秒スリープ

        // // ボタンが離されるまで待機
        // while(robot.getButtonInstance().isRightPressed()) {
        //   robot.getClockInstance().sleep(10000);  // 10ミリ秒スリープ
        // }
        break;
      }
    }

    robot.getClockInstance().sleep(10000);  // 10ミリ秒スリープ
  }

  targetBrightness = (whiteBrightness + blackBrightness) / 2;
  // 目標輝度をディスプレイに表示
  robot.getDisplayInstance().showNumber(targetBrightness);
  cout << "Target Brightness Value is " << targetBrightness << endl;
  robot.getClockInstance().sleep(100000);  // 1秒スリープ
  robot.getDisplayInstance().off();
}

void Calibrator::waitForStart()
{
  printf("On standby.\n");
  // ForceSensorが押されるまで待機
  while(!robot.getForceSensorInstance().isPressed(PRESS_POWER)) {
    robot.getClockInstance().sleep(10000);  // 10ミリ秒スリープ
  }
}

bool Calibrator::getIsLeftCourse()
{
  return isLeftCourse;
}

int Calibrator::getTargetBrightness()
{
  return targetBrightness;
}