#include "MotorController.h"
#include <string>
#include <vector>
#include "../common/StringOperator.h"

MotorController::MotorController(SpikeClient& client)
  : spikeClient(client)
{
}

// 右モータにpower値をセット
void MotorController::setRightMotorPower(int power)
{
  std::string command = "motor,set_power,right,";
  command += std::to_string(power);
  spikeClient.sendCommand(command);
}

// 左モータにpower値をセット
void MotorController::setLeftMotorPower(int power)
{
  std::string command = "motor,set_power,left,";
  command += std::to_string(power);
  spikeClient.sendCommand(command);
}

// 右モータのpower値をリセット
void MotorController::resetRightMotorPower()
{
  setRightMotorPower(0);
}

// 左モータのpower値をリセット
void MotorController::resetLeftMotorPower()
{
  setLeftMotorPower(0);
}

// 右左両モータの状態をリセット
void MotorController::resetWheelsMotorPower()
{
  resetRightMotorPower();
  resetLeftMotorPower();
}

// 右タイヤのモータに,線速度を回転速度に変換しセットする
void MotorController::setRightMotorSpeed(double speed)
{
  std::string command = "motor,set_speed,right,";
  command += std::to_string(speed);
  spikeClient.sendCommand(command);
}

// 左タイヤのモータに,線速度を回転速度に変換しセットする
void MotorController::setLeftMotorSpeed(double speed)
{
  std::string command = "motor,set_speed,left,";
  command += std::to_string(speed);
  spikeClient.sendCommand(command);
}

// 両タイヤのモータを停止する
void MotorController::stopWheelsMotor()
{
  spikeClient.sendCommand("motor,stop_wheels");
}

// ブレーキをかけてタイヤのモータを停止する
void MotorController::brakeWheelsMotor()
{
    spikeClient.sendCommand("motor,brake_wheels");
}

// アームのモータにpower値をセット
void MotorController::setArmMotorPower(int power)
{
  std::string command = "motor,set_power,arm,";
  command += std::to_string(power);
  spikeClient.sendCommand(command);
}

// アームのモータのpower値をリセット
void MotorController::resetArmMotorPower()
{
  setArmMotorPower(0);
}

// アームのモータを停止する
void MotorController::stopArmMotor()
{
  spikeClient.sendCommand("motor,stop_arm");
}

// アームモータを止めて角度を維持する
void MotorController::holdArmMotor()
{
  spikeClient.sendCommand("motor,hold_arm");
}

// 右モータの角位置を取得する
int32_t MotorController::getRightMotorCount()
{
  std::string res = spikeClient.sendCommand("motor,get_count,right");
  return std::stoi(res);
}

// 左モータの角位置を取得する
int32_t MotorController::getLeftMotorCount()
{
  std::string res = spikeClient.sendCommand("motor,get_count,left");
  return std::stoi(res);
}

// アームモータの角位置を取得する
int32_t MotorController::getArmMotorCount()
{
  std::string res = spikeClient.sendCommand("motor,get_count,arm");
  return std::stoi(res);
}

// 右タイヤのpower値を取得する
int MotorController::getRightMotorPower()
{
    std::string res = spikeClient.sendCommand("motor,get_power,right");
    return std::stoi(res);
}

// 左タイヤのpower値を取得する
int MotorController::getLeftMotorPower()
{
    std::string res = spikeClient.sendCommand("motor,get_power,left");
    return std::stoi(res);
}

// アームモータのpower値を取得する
int MotorController::getArmMotorPower()
{
    std::string res = spikeClient.sendCommand("motor,get_power,arm");
    return std::stoi(res);
}

// 右タイヤモータの線速度を取得する
double MotorController::getRightMotorSpeed()
{
    std::string res = spikeClient.sendCommand("motor,get_speed,right");
    return std::stod(res);
}

// 左タイヤモータの線速度を取得する
double MotorController::getLeftMotorSpeed()
{
    std::string res = spikeClient.sendCommand("motor,get_speed,left");
    return std::stod(res);
}