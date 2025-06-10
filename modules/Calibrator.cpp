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
  const char* text1 = "SELECT A COURSE";
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
  const char* text2 = "OK!";
  const char* course = isLeftCourse ? "Left" : "Right";
  cout << "Will Run on the" << course << " Course" << endl;
  robot.getDisplayInstance().scrollText(text2, 50);
  robot.getClockInstance().sleep(2000000);  // 2秒スリープ
}

void Calibrator::measureAndSetTargetBrightness()
{
  u_int8_t blackBrightness = -1;
  u_int8_t whiteBrightness = -1;
  u_int8_t targetBrightness = -1;

  const char* text1 = "BLACK";
  robot.getDisplayInstance().scrollText(text1, 50);
  // 黒の輝度測定
  // 左ボタンで輝度を取得し、右ボタンで黒の輝度を決定する
  while(1) {
    // 左ボタンが押されたら輝度を取得
    if(robot.getButtonInstance().isLeftPressed()) {
      blackBrightness = robot.getColorSensorInstance().getReflection();

      // 黒の輝度を取得したことをディスプレイに表示
      robot.getDisplayInstance().showNumber(blackBrightness);
      robot.getClockInstance().sleep(10000);  // 10ミリ秒スリープ

      // // ボタンが離されるまで待機
      // while(robot.getButtonInstance().isLeftPressed()) {
      //   robot.getClockInstance().sleep(10000);  // 10ミリ秒スリープ
      // }
    }

    // 輝度が取得済み、かつ右ボタンが押されたら確定
    if(blackBrightness >= 0 && robot.getButtonInstance().isRightPressed()) {
      // 黒の輝度を確定したことをディスプレイに表示
      const char* text2 = "OK!";
      robot.getDisplayInstance().scrollText(text2, 50);
      robot.getClockInstance().sleep(200000);  // 2秒スリープ

      // // ボタンが離されるまで待機
      // while(robot.getButtonInstance().isRightPressed()) {
      //   robot.getClockInstance().sleep(10000);  // 10ミリ秒スリープ
      // }
      break;
    }
  }

  const char* text3 = "WHITE";
  robot.getDisplayInstance().scrollText(text3, 50);
  // 白の輝度測定
  // 左ボタンで輝度を取得し、右ボタンで白の輝度を決定する
  while(1) {
    // 左ボタンが押されたら輝度を取得
    if(robot.getButtonInstance().isLeftPressed()) {
      whiteBrightness = robot.getColorSensorInstance().getReflection();

      // 輝度を取得したことをディスプレイに表示
      robot.getDisplayInstance().showNumber(whiteBrightness);
      robot.getClockInstance().sleep(10000);  // 10ミリ秒スリープ

      // // ボタンが離されるまで待機
      // while(robot.getButtonInstance().isLeftPressed()) {
      //   robot.getClockInstance().sleep(10000);  // 10ミリ秒スリープ
      // }
    }

    // 輝度が取得済み、かつ右ボタンが押されたら確定
    if(whiteBrightness >= 0 && robot.getButtonInstance().isRightPressed()) {
      // 白の輝度を確定したことをディスプレイに表示
      const char* text4 = "OK!";
      robot.getDisplayInstance().scrollText(text4, 50);
      robot.getClockInstance().sleep(200000);  // 2秒スリープ

      // // ボタンが離されるまで待機
      // while(robot.getButtonInstance().isRightPressed()) {
      //   robot.getClockInstance().sleep(10000);  // 10ミリ秒スリープ
      // }
      break;
    }
  }

  targetBrightness = (whiteBrightness + blackBrightness) / 2;
  // 目標輝度をディスプレイに表示
  robot.getDisplayInstance().showNumber(targetBrightness);
  cout << "Target Brightness Value is" << targetBrightness << endl;
  robot.getClockInstance().sleep(100000);  // 1秒スリープ
  robot.getDisplayInstance().off();
}

void Calibrator::waitForStart()
{
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