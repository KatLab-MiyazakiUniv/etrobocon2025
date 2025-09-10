// /**
//  * @file   ColorDistanceStraight.h
//  * @brief  目標距離まで直進するクラス
//  * @author nishijima515
//  */

// #ifndef COLOR_DISTANCE_STRAIGHT_H
// #define COLOR_DISTANCE_STRAIGHT_H

// #include "Straight.h"
// #include "Mileage.h"

// class ColorDistanceStraight : public Straight {
//  public:
//   /**
//    * @brief コンストラクタ
//    * @param _robot ロボット本体への参照
//    * @param _targetDistance 目標距離 [mm]
//    * @param _speed   目標速度[mm/s]
//    */
//   ColorDistanceStraight(Robot& _robot, double _targetDistance, double _speed);

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
//   double targetDistance;   // 目標距離 [mm]
//   double initialDistance;  // 実行前の走行距離
// };

// #endif