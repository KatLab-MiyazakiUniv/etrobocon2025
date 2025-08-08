#include "Rotation.h"
#include <iostream>
#include <thread>
#include <chrono>

Rotation::Rotation(Robot& _robot, bool _isClockwise, int _targetAngle, const PidGain& _pidGain)
  : Motion(_robot),
    leftSign(_isClockwise ? 1 : -1),
    rightSign(_isClockwise ? -1 : 1),
    targetAngle(_targetAngle),
    pidGain(_pidGain)
{
}

void Rotation::run()
{
    MotorController& motorController = robot.getMotorControllerInstance();
    spikeapi::IMU& imu = robot.getIMUInstance();

    prepare();

    if(!isMetPreCondition()) {
        std::cerr << "Rotation::run(): pre-condition not met. Abort." << std::endl;
        return;
    }

    // 目標角速度 [deg/s]、回転方向に応じて符号をつける
    double targetAngularVelocity = 30.0 * rightSign;

    // PID制御器（角速度に対して制御）
    Pid pidAngularVelocity(pidGain.kp, pidGain.ki, pidGain.kd, targetAngularVelocity);

    while(true) {
        // IMUから角速度Z軸（回転方向）を取得
        spikeapi::IMU::AngularVelocity angVel;
        imu.getAngularVelocity(angVel);
        double currentAngularVelocity = angVel.z;
        // デバッグ出力
        std::cout << "Current Angular Velocity: " << currentAngularVelocity << " deg/s" << std::endl;

        // PIDで制御量計算（現在角速度に対するモーター出力の目標調整）
        double controlOutput = pidAngularVelocity.calculatePid(currentAngularVelocity);

        // 左右モーターに出力、回転方向に合わせて反転を入れる
        motorController.setLeftMotorPower(controlOutput * leftSign);
        motorController.setRightMotorPower(controlOutput * rightSign);

        // 角度の経過を判定（時間か角度の誤差で終了判定する）
        if(!isMetContinuationCondition()) {
            break;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    std::cout << "Rotation completed. Stopping motors." << std::endl;
    motorController.brakeWheelsMotor();
}

