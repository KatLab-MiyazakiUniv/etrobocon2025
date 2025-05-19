/**
 * @file Port.h
 * @brief ポート関連定義
 * @author HaruArima08
 */

#ifndef PORT_H
#define PORT_H

#include "spikeapi.h"
#include "pbio/port.h"

/**
 * モータ/センサポート関連定義
 */

enum class Port {
  PORT_A = PBIO_PORT_ID_A, /**< SPIKE ポートA */
  PORT_B = PBIO_PORT_ID_B, /**< SPIKE ポートB */
  PORT_C = PBIO_PORT_ID_C, /**< SPIKE ポートC */
  PORT_D = PBIO_PORT_ID_D, /**< SPIKE ポートD */
  PORT_E = PBIO_PORT_ID_E, /**< SPIKE ポートE */
  PORT_F = PBIO_PORT_ID_F  /**< SPIKE ポートF */
};

#endif