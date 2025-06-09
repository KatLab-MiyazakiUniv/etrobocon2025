/**
 * @file Calibrator.cpp
 * @brief キャリブレーションからスタートまでを担当するクラス
 * @author HaruArima08 nishijima515
 */

#include "Calibrator.h"

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
  // 右ボタンが押されたら確定する
  while(!robot.getButtonInstance().isRightPressed()) {
    if(robot.getButtonInstance().isLeftPressed() && !isLeftCourse) {
      // 左ボタンが押されたときRコースがセットされていれば、Lコースをセットする
      isLeftCourse = true;
      isLeftEdge = true;
      robot.setIsLeftEdge(isLeftEdge);  // Lコースを選択したので、左エッジとする
      // 画面にLコースが選択されたことを表示
      robot.getDisplayInstance().off();
      robot.getDisplayInstance().showChar(LEFT);
      robot.getClockInstance().sleep(500);  // 500ミリ秒スリープ
    } else if(robot.getButtonInstance().isLeftPressed() && isLeftCourse) {
      // 左ボタンが押されたときLコースがセットされていれば、Rコースをセットする
      isLeftCourse = false;
      isLeftEdge = false;
      robot.setIsLeftEdge(isLeftEdge);  // Rコースを選択したので、右エッジとする
      // 画面にRコースが選択されたことを表示
      robot.getDisplayInstance().off();
      robot.getDisplayInstance().showChar(RIGHT);
      robot.getClockInstance().sleep(500);  // 500ミリ秒スリープ
    } else {
      robot.getClockInstance().sleep(10000);  // 10ミリ秒スリープ
    }
  }
  robot.getClockInstance().sleep(1000);  // 1秒スリープ
}

void Calibrator::measureAndSetTargetBrightness()
{
  int whiteBrightness = -1;
  int blackBrightness = -1;
  targetBrightness = -1;

  // 黒の輝度測定
  // 左ボタンで輝度を取得し、右ボタンで黒の輝度を決定する
  while(1) {
    // 左ボタンが押されたら輝度を取得
    if(robot.getButtonInstance().isLeftPressed()) {
      blackBrightness = robot.getColorSensorInstance().getReflection();

      // 輝度を取得したことをディスプレイに表示
      robot.getDisplayInstance().off();
      robot.getDisplayInstance().showChar(OK);
      robot.getClockInstance().sleep(500);  // 500ミリ秒スリープ

      // ボタンが離されるまで待機
      while(robot.getButtonInstance().isLeftPressed()) {
        robot.getClockInstance().sleep(10000);  // 10ミリ秒スリープ
      }
    }

    // 輝度が取得済み、かつ右ボタンが押されたら確定
    if(blackBrightness >= 0 && robot.getButtonInstance().isRightPressed()) {
      // 黒の輝度を確定したことをディスプレイに表示
      robot.getDisplayInstance().off();
      robot.getDisplayInstance().showChar(FINISH);
      robot.getClockInstance().sleep(500);  // 500ミリ秒スリープ

      // ボタンが離されるまで待機
      while(robot.getButtonInstance().isLeftPressed()) {
        robot.getClockInstance().sleep(10000);  // 10ミリ秒スリープ
      }
    }
    break;
  }

  // 白の輝度測定
  // 左ボタンで輝度を取得し、右ボタンで白の輝度を決定する
  while(1) {
    // 左ボタンが押されたら輝度を取得
    if(robot.getButtonInstance().isLeftPressed()) {
      whiteBrightness = robot.getColorSensorInstance().getReflection();

      // 輝度を取得したことをディスプレイに表示
      robot.getDisplayInstance().off();
      robot.getDisplayInstance().showChar(OK);
      robot.getClockInstance().sleep(500);  // 500ミリ秒スリープ

      // ボタンが離されるまで待機
      while(robot.getButtonInstance().isLeftPressed()) {
        robot.getClockInstance().sleep(10000);  // 10ミリ秒スリープ
      }
    }

    // 輝度が取得済み、かつ右ボタンが押されたら確定
    if(blackBrightness >= 0 && robot.getButtonInstance().isRightPressed()) {
      // 白の輝度を確定したことをディスプレイに表示
      robot.getDisplayInstance().off();
      robot.getDisplayInstance().showChar(FINISH);
      robot.getClockInstance().sleep(500);  // 500ミリ秒スリープ

      // ボタンが離されるまで待機
      while(robot.getButtonInstance().isLeftPressed()) {
        robot.getClockInstance().sleep(10000);  // 10ミリ秒スリープ
      }
    }
    break;
  }

  targetBrightness = (whiteBrightness + blackBrightness) / 2;
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