/**
 * @file   RotationMatrix.h
 * @brief  3D回転行列計算クラス
 * @author Hara1274 takuchi17
 */

#ifndef ROTATION_MATRIX_H
#define ROTATION_MATRIX_H

#include <cmath>

/**
 * @brief 3D回転行列計算クラス
 */
class RotationMatrix {
 public:
  /**
   * @brief ロドリゲスの回転公式による3D回転行列を計算する
   * @param acc_x X軸加速度
   * @param acc_y Y軸加速度
   * @param acc_z Z軸加速度
   * @param matrix 計算結果を格納する3x3行列
   */
  static void calculateCorrectionMatrix(float acc_x, float acc_y, float acc_z,
                                        float (&matrix)[3][3]);
};

#endif