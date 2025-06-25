/**
 * @file SystemInfo.h
 * @brief 走行システムで統一する情報をまとめたファイル
 * @author molpui0726, nishijima515
 */

#ifndef SYSTEM_INFO_H
#define SYSTEM_INFO_H

#define PI 3.1415926           // 円周率
#define RAD_TO_DEG 180.0 / PI  // ラジアンを度数に変換するための係数
#define DEG_TO_RAD PI / 180.0  // 度数をラジアンに変換するための係数

static constexpr double TREAD = 112.0;  // 走行体のトレッド幅（両輪の間の距離）[mm]

static constexpr double WHEEL_RADIUS = 28.0;  // 車輪の半径[mm]

static const int RESOLUTION_WIDTH = 800;   // 解像度の幅（規定値）[px]
static const int RESOLUTION_HEIGHT = 600;  // 解像度の高さ（規定値）[px]
static constexpr int MIN_WIDTH = 320;      // 解像度の幅（最小値）[px]
static constexpr int MIN_HEIGHT = 240;     // 解像度の高さ（最小値）[px]
static constexpr int MAX_WIDTH = 1920;     // 解像度の幅（最大値）[px]
static constexpr int MAX_HEIGHT = 1080;    // 解像度の高さ（最大値）[px]
#endif
