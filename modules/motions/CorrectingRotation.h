// /**
//  * @file   CorrectingRotation.h
//  * @brief  補正回頭動作の親クラス
//  * @author nishijima515
//  */

// #ifndef CORRECTING_ROTATION_H
// #define CORRECTING_ROTATION_H

// #include "Motion.h"
// #include "Pid.h"
// #include "SpeedCalculator.h"
// #include "Mileage.h"
// #include <algorithm>
// #include "SocketProtocol.h"

// class CorrectingRotation : public Motion {
//  public:
//   /**
//    * コンストラクタ
//    * @param _robot       ロボット制御クラスへの参照
//    * @param _isClockwise 回頭方向 true:時計回り, false:反時計回り
//    */
//   CorrectingRotation(Robot& _robot, int _targetXCoordinate, const PidGain& _pidGain,
//                      const CameraServer::BoundingBoxDetectorRequest& _detectionRequest);

//   /**
//    * @brief 回頭する
//    */
//   void run();

//   /**
//    * @brief 回頭動作の事前準備を行う
//    * @note オーバーライド必須
//    */
//   virtual void prepare() = 0;

//   /**
//    * @brief 回頭する際の事前条件判定をする
//    * @return true: 事前条件を満たす,false: 事前条件を満たさない
//    * @note オーバーライド必須
//    */
//   virtual bool isMetPreCondition() = 0;

//   /**
//    * @brief 回頭する際の継続条件判定をする。返り値がfalseでモーターが止まる
//    * @return true: 継続, false: 停止（モーター停止）
//    * @note オーバーライド必須
//    */
//   virtual bool isMetContinuationCondition() = 0;

//   /**
//    * @brief 継続中にモーターを動的制御する
//    * @note オーバーライド必須
//    */
//   virtual void updateMotorControl() {}

//  protected:
//   int targetXCoordinate;                                            // 目標X座標
//   const PidGain pidGain;                                            // PIDゲイン
//   const CameraServer::BoundingBoxDetectorRequest detectionRequest;  // 検出リクエスト
// };
// #endif