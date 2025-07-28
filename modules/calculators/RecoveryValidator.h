/**
 * @file   RecoveryValidator.h
 * @brief  復帰動作時の位置変化検証クラス
 * @author HaruArima08
 */

#ifndef SIMPLE_RECOVERY_VALIDATOR_H
#define SIMPLE_RECOVERY_VALIDATOR_H

class RecoveryValidator {
 public:
  /**
   * コンストラクタ
   * @param _maxCenterXChange 最大中心X座標変化量
   */
  RecoveryValidator(double _maxCenterXChange);

  /**
   * @brief 復帰動作前の中心X座標を保存する
   * @param centerX 復帰動作直前の中心X座標
   */
  void savePreRecoveryPosition(double centerX);

  /**
   * @brief 復帰後の位置変化が範囲内かチェック
   * @param currentX 復帰後の中心X座標
   * @return true: 妥当な位置変化, false: 異常な位置変化
   */
  bool isPositionChangeValid(double currentX);

  /**
   * @brief 保存された復帰前位置をクリアする
   */
  void clear();

 private:
  double prevXCoordinate;          // 復帰動作前の中心X座標
  double maxCenterXChange = 80.0;  // 最大中心X座標変化量
  bool hasPreRecoveryData;         // 復帰前データの有無
};

#endif