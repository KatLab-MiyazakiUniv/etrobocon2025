// /**
//  * @file   IMUColorDistanceStraight.h
//  * @brief  IMU角度補正を用いた目標色距離まで直進するクラス
//  * @author nishijima515
//  */

// #ifndef IMU_COLOR_DISTANCE_STRAIGHT_H
// #define IMU_COLOR_DISTANCE_STRAIGHT_H

// #include "Straight.h"
// #include "ColorJudge.h"
// #include "Mileage.h"
// #include "Pid.h"

// class IMUColorDistanceStraight : public Straight {
//  public:
//   /**
//    * @brief コンストラクタ
//    * @param _robot ロボット本体への参照
//    * @param _targetDistance 目標距離 [mm]
//    * @param _targetSpeed   目標速度[mm/s]
//    * @param _anglePidGain 角度制御PIDゲイン
//    */
//   IMUColorDistanceStraight(Robot& _robot, COLOR _targetColor, double _targetDistance,
//                            double _targetSpeed, const PidGain& _anglePidGain);

//   /**
//    * @brief 直進する
//    */
//   void run() override;

//  protected:
//   /**
//    * @brief 直進する際の事前条件判定をする
//    */
//   bool isMetPreCondition() override;

//   /**
//    * @brief 直進する際の事前処理をする
//    */
//   void prepare() override;

//   /**
//    * @brief 直進する際の動作継続条件判定をする 返り値がtrueの間モーターが回転
//    */
//   bool isMetContinuationCondition() override;

//  private:
//   double targetDistance;                 // 目標距離 [mm]
//   double initialDistance;                // 実行前の走行距離
//   Pid anglePid;                          // 角度制御PID
//   double targetAngle;                    // 目標角度
//   static constexpr int JUDGE_COUNT = 2;  // 色取得の決定に必要な連続回数
//   int colorCount;                        // 指定色を取得した回数
//   COLOR targetColor;                     // 指定色
// };

// #endif
