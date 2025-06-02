/**
 * @file Port.h
 * @brief ポート関連定義(ダミー)
 * @author nishijima515
 */

#ifndef SPIKE_CPP_API_PORT_H_
#define SPIKE_CPP_API_PORT_H_

/** センサポート数 */
#define NUM_PORT_S (6)  // number of sensor ports

/** モータポート数 */
#define NUM_PORT_M (6)  // number of motor ports

/**
 * モータ/センサポート関連定義
 */

/**
 * モータ/センサポート番号
 */
enum class EPort {
  PORT_A = 0, /**< SPIKE ポートA */
  PORT_B = 1, /**< SPIKE ポートB */
  PORT_C = 2, /**< SPIKE ポートC */
  PORT_D = 3, /**< SPIKE ポートD */
  PORT_E = 4, /**< SPIKE ポートE */
  PORT_F = 5  /**< SPIKE ポートF */
};

#endif  // ! SPIKE_CPP_API_PORT_H_
