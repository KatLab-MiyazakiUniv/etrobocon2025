/**
 * @file Calibrator.cpp
 * @brief キャリブレーションからスタートまでを担当するクラス
 * @author HaruArima08 nishijima515
 */

#include "Calibrator.h"
#include <thread>
#include <chrono>
#include <iostream>

using namespace std;

Calibrator::Calibrator(Robot& _robot) : robot(_robot), isLeftCourse(true), targetBrightness(50) {}

void Calibrator::selectAndSetCourse()
{
  robot.getDisplayInstance().showChar('C');  // C:コース
  while(1) {
    // 左ボタンが押されたときRコースがセットされていれば、Lコースをセットする
    if(robot.getButtonInstance().isLeftPressed() && !isLeftCourse) {
      isLeftCourse = true;
      // 画面にLコースが選択されたことを表示
      robot.getDisplayInstance().showChar('L');
      std::this_thread::sleep_for(std::chrono::milliseconds(10));  // 10ミリ秒スリープ
      // ボタンが離されるまで待機
      while(robot.getButtonInstance().isLeftPressed()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));  // 10ミリ秒スリープ
      }
    }
    // 左ボタンが押されたときLコースがセットされていれば、Rコースをセットする
    else if(robot.getButtonInstance().isLeftPressed() && isLeftCourse) {
      isLeftCourse = false;
      // 画面にRコースが選択されたことを表示
      robot.getDisplayInstance().showChar('R');
      std::this_thread::sleep_for(std::chrono::milliseconds(10));  // 10ミリ秒スリープ
      // ボタンが離されるまで待機
      while(robot.getButtonInstance().isLeftPressed()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));  // 10ミリ秒スリープ
      }
    }
    // 右ボタンが押されたときコース選択を終了
    if(robot.getButtonInstance().isRightPressed()) {
      break;
    }
  }
  const char* course = isLeftCourse ? "Left" : "Right";
  printf("\nWill Run on the %s Course\n", course);
  robot.getDisplayInstance().scrollText("OK", 50);
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));  // 1秒スリープ
}

void Calibrator::measureAndSetTargetBrightness()
{
  int blackBrightness = -1;
  int whiteBrightness = -1;

  robot.getDisplayInstance().showChar('B');
  // 黒の輝度測定
  // 左ボタンで輝度を取得し、右ボタンで黒の輝度を決定する
  bool isBrightnessMeasuring = false;
  while(1) {
    // 左ボタンが押されたら輝度値の連続取得を開始
    if(robot.getButtonInstance().isLeftPressed() && !isBrightnessMeasuring) {
      isBrightnessMeasuring = true;
      // ボタンが離されるまで待機
      while(robot.getButtonInstance().isLeftPressed()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));  // 10ミリ秒スリープ
      }
    }
    // ループが開始されたら連続的に輝度を取得
    if(isBrightnessMeasuring) {
      blackBrightness = robot.getColorSensorInstance().getReflection();
      // 現在の輝度値をリアルタイムで表示
      robot.getDisplayInstance().showNumber(blackBrightness);
      // 右ボタンが押されたら確定
      if(robot.getButtonInstance().isRightPressed()) {
        // 黒の輝度を確定したことをディスプレイに表示
        robot.getDisplayInstance().scrollText("OK", 50);
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));  // 1秒スリープ
        // ボタンが離されるまで待機
        while(robot.getButtonInstance().isRightPressed()) {
          std::this_thread::sleep_for(std::chrono::milliseconds(10));  // 10ミリ秒スリープ
        }
        break;
      }
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(10));  // 10ミリ秒スリープ
  }

  robot.getDisplayInstance().showChar('W');
  // 白の輝度測定
  // 左ボタンで輝度を取得し、右ボタンで白の輝度を決定する
  isBrightnessMeasuring = false;
  while(1) {
    // 左ボタンが押されたら輝度値の連続取得を開始
    if(robot.getButtonInstance().isLeftPressed() && !isBrightnessMeasuring) {
      isBrightnessMeasuring = true;
      // ボタンが離されるまで待機
      while(robot.getButtonInstance().isLeftPressed()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));  // 10ミリ秒スリープ
      }
    }
    // ループが開始されたら連続的に輝度を取得
    if(isBrightnessMeasuring) {
      whiteBrightness = robot.getColorSensorInstance().getReflection();
      // 現在の輝度値をリアルタイムで表示
      robot.getDisplayInstance().showNumber(whiteBrightness);
      // 右ボタンが押されたら確定
      if(robot.getButtonInstance().isRightPressed()) {
        // 白の輝度を確定したことをディスプレイに表示
        robot.getDisplayInstance().scrollText("OK", 50);
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));  // 1秒スリープ
        // ボタンが離されるまで待機
        while(robot.getButtonInstance().isRightPressed()) {
          std::this_thread::sleep_for(std::chrono::milliseconds(10));  // 10ミリ秒スリープ
        }
        break;
      }
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(10));  // 10ミリ秒スリープ
  }

  targetBrightness = (whiteBrightness + blackBrightness) / 2;
  // 目標輝度をディスプレイに表示
  robot.getDisplayInstance().showNumber(targetBrightness);
  cout << "Target Brightness Value is " << targetBrightness << endl;
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));  // 1秒スリープ
  robot.getDisplayInstance().showChar(' ');                      // ディスプレイを消灯
}

void Calibrator::getAngleCheckFrame()
{
  robot.getDisplayInstance().showChar('F');
  // 角度調整フレーム取得
  // 中央ボタンでフレームを取得し、右ボタンで決定する
  std::string fileName = "angleCheckFrame.jpg";
  while(1) {
    // 中央ボタンが押されたらフレーム取得
    if(robot.getButtonInstance().isCenterPressed()) {
      // ボタンが離されるまで待機
      while(robot.getButtonInstance().isCenterPressed()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));  // 10ミリ秒スリープ
      }
      Snapshot snapshot(robot, fileName);
      snapshot.run();
      robot.getDisplayInstance().showChar('S');
      std::this_thread::sleep_for(std::chrono::milliseconds(10));  // 10ミリ秒スリープ
    }
    // 右ボタンで終了
    if(robot.getButtonInstance().isRightPressed()) {
      robot.getDisplayInstance().scrollText("OK", 50);
      std::this_thread::sleep_for(std::chrono::milliseconds(1000));  // 1秒スリープ
      // ボタンが離されるまで待機
      while(robot.getButtonInstance().isRightPressed()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));  // 10ミリ秒スリープ
      }
      break;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(10));  // 10ミリ秒スリープ
  }
  cout << "Complete Check Frame." << endl;
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));  // 1秒スリープ
}

void Calibrator::waitForStart()
{
  printf("On standby.\n");
  // ForceSensorが押されるまで待機
  while(!robot.getForceSensorInstance().isPressed(PRESS_POWER)) {
    std::this_thread::sleep_for(std::chrono::milliseconds(10));  // 10ミリ秒スリープ
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