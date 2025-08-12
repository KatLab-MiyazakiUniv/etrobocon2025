#include "MotorControllerClient.h"
#include <string>
#include <vector>
#include "../common/StringOperator.h"

MotorControllerClient::MotorControllerClient(SpikeClient& client)
  : spikeClient(client)
{
}

// 右モータにpower値をセット
void MotorControllerClient::setRightMotorPower(int power)
{
    spike::MotorSetPowerRequest req{ spike::MotorTarget::RIGHT, power };
    spikeClient.executeCommand(
        spike::CommandId::MOTOR_SET_POWER,
        req
    );
}

// 左モータにpower値をセット
void MotorControllerClient::setLeftMotorPower(int power)
{
    spike::MotorSetPowerRequest req{ spike::MotorTarget::LEFT, power };
    spikeClient.executeCommand(
        spike::CommandId::MOTOR_SET_POWER,
        req
    );
}

// 右モータのpower値をリセット
void MotorControllerClient::resetRightMotorPower()
{
  setRightMotorPower(0);
}

// 左モータのpower値をリセット
void MotorControllerClient::resetLeftMotorPower()
{
  setLeftMotorPower(0);
}

// 右左両モータの状態をリセット
void MotorControllerClient::resetWheelsMotorPower()
{
  resetRightMotorPower();
  resetLeftMotorPower();
}

// 右タイヤのモータに,線速度を回転速度に変換しセットする
void MotorControllerClient::setRightMotorSpeed(double speed)
{
    spike::MotorSetSpeedRequest req{ spike::MotorTarget::RIGHT, speed };
    spikeClient.executeCommand(
        spike::CommandId::MOTOR_SET_SPEED,
        req
    );
}

// 左タイヤのモータに,線速度を回転速度に変換しセットする
void MotorControllerClient::setLeftMotorSpeed(double speed)
{
    spike::MotorSetSpeedRequest req{ spike::MotorTarget::LEFT, speed };
    spikeClient.executeCommand(
        spike::CommandId::MOTOR_SET_SPEED,
        req
    );
}

// 両タイヤのモータを停止する
void MotorControllerClient::stopWheelsMotor()
{
    spikeClient.executeCommand(
        spike::CommandId::MOTOR_STOP_WHEELS
    );
}

// ブレーキをかけてタイヤのモータを停止する
void MotorControllerClient::brakeWheelsMotor()
{
    spikeClient.executeCommand(
        spike::CommandId::MOTOR_BRAKE_WHEELS
    );
}

// アームのモータにpower値をセット
void MotorControllerClient::setArmMotorPower(int power)
{
    spike::MotorSetPowerRequest req{ spike::MotorTarget::ARM, power };
    spikeClient.executeCommand(
        spike::CommandId::MOTOR_SET_POWER,
        req
    );
}

// アームのモータのpower値をリセット
void MotorControllerClient::resetArmMotorPower()
{
  setArmMotorPower(0);
}

// アームのモータを停止する
void MotorControllerClient::stopArmMotor()
{
    spikeClient.executeCommand(
        spike::CommandId::MOTOR_STOP_ARM
    );
}

// アームモータを止めて角度を維持する
void MotorControllerClient::holdArmMotor()
{
    spikeClient.executeCommand(
        spike::CommandId::MOTOR_HOLD_ARM
    );
}

// 右モータの角位置を取得する
int32_t MotorControllerClient::getRightMotorCount()
{
    auto res = spikeClient.executeQuery<int32_t>(
        spike::CommandId::MOTOR_GET_COUNT,
        spike::MotorTarget::RIGHT
    );
    return res.value_or(0);
}

// 左モータの角位置を取得する
int32_t MotorControllerClient::getLeftMotorCount()
{
    auto res = spikeClient.executeQuery<int32_t>(
        spike::CommandId::MOTOR_GET_COUNT,
        spike::MotorTarget::LEFT
    );
    return res.value_or(0);
}

// アームモータの角位置を取得する
int32_t MotorControllerClient::getArmMotorCount()
{
    auto res = spikeClient.executeQuery<int32_t>(
        spike::CommandId::MOTOR_GET_COUNT,
        spike::MotorTarget::ARM
    );
    return res.value_or(0);
}

// 右タイヤのpower値を取得する
int MotorControllerClient::getRightMotorPower()
{
    auto res = spikeClient.executeQuery<int32_t>(
        spike::CommandId::MOTOR_GET_POWER,
        spike::MotorTarget::RIGHT
    );
    return res.value_or(0);
}

// 左タイヤのpower値を取得する
int MotorControllerClient::getLeftMotorPower()
{
    auto res = spikeClient.executeQuery<int32_t>(
        spike::CommandId::MOTOR_GET_POWER,
        spike::MotorTarget::LEFT
    );
    return res.value_or(0);
}

// アームモータのpower値を取得する
int MotorControllerClient::getArmMotorPower()
{
    auto res = spikeClient.executeQuery<int32_t>(
        spike::CommandId::MOTOR_GET_POWER,
        spike::MotorTarget::ARM
    );
    return res.value_or(0);
}

// 右タイヤモータの線速度を取得する
double MotorControllerClient::getRightMotorSpeed()
{
    auto res = spikeClient.executeQuery<double>(
        spike::CommandId::MOTOR_GET_SPEED,
        spike::MotorTarget::RIGHT
    );
    return res.value_or(0.0);
}

// 左タイヤモータの線速度を取得する
double MotorControllerClient::getLeftMotorSpeed()
{
    auto res = spikeClient.executeQuery<double>(
        spike::CommandId::MOTOR_GET_SPEED,
        spike::MotorTarget::LEFT
    );
    return res.value_or(0.0);
}