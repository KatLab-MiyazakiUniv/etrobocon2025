/**
 * @file   RotationMatrix.cpp
 * @brief  3D回転行列計算クラス
 * @author Hara1274 takuchi17
 */

#include "RotationMatrix.h"

void RotationMatrix::calculateCorrectionMatrix(float acc_x, float acc_y, float acc_z,
                                               float (&matrix)[3][3])
{
  // 正規化
  float norm = std::sqrt(acc_x * acc_x + acc_y * acc_y + acc_z * acc_z);
  float gx = acc_x / norm;
  float gy = acc_y / norm;
  float gz = acc_z / norm;

  // 理想の重力方向（Z軸）
  float ez[3] = { 0.0f, 0.0f, 1.0f };

  // 回転軸 v = ez × g（外積）
  float vx = ez[1] * gz - ez[2] * gy;  // 0*gz - 1*gy = -gy
  float vy = ez[2] * gx - ez[0] * gz;  // 1*gx - 0*gz = gx
  float vz = ez[0] * gy - ez[1] * gx;  // 0*gy - 0*gx = 0

  // 回転角 θ = acos(ez・g)（内積）
  float dot = ez[0] * gx + ez[1] * gy + ez[2] * gz;
  float theta = std::acos(dot);

  // 回転軸を正規化
  float v_norm = std::sqrt(vx * vx + vy * vy + vz * vz);
  vx /= v_norm;
  vy /= v_norm;
  vz /= v_norm;

  float c = std::cos(theta);
  float s = std::sin(theta);
  float t = 1 - c;

  // ロドリゲスの回転公式による3D回転行列を計算
  matrix[0][0] = t * vx * vx + c;
  matrix[0][1] = t * vx * vy - s * vz;
  matrix[0][2] = t * vx * vz + s * vy;
  matrix[1][0] = t * vx * vy + s * vz;
  matrix[1][1] = t * vy * vy + c;
  matrix[1][2] = t * vy * vz - s * vx;
  matrix[2][0] = t * vx * vz - s * vy;
  matrix[2][1] = t * vy * vz + s * vx;
  matrix[2][2] = t * vz * vz + c;
}