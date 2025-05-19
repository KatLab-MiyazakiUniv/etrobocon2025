/**
 * @file motor.h
 * @brief モータクラス(ダミー)
 * @author takahashitom
 */

#ifndef _MOTOR_H_
#define _MOTOR_H_

#include <pbio/port.h>
#include <cstdint>

typedef enum {
  PUP_DIRECTION_CLOCKWISE = 0,
  PUP_DIRECTION_COUNTERCLOCKWISE = 1,
} pup_direction_t;

typedef enum {
  PBIO_SUCCESS = 0,
  PBIO_ERROR = -1,
} pbio_error_t;

struct pup_motor_t {
  int count = 0;
  int speed = 0;
  int power = 0;
  int duty_limit = 100;
  bool is_stalled = false;
  pup_direction_t direction = PUP_DIRECTION_CLOCKWISE;
};

static pup_motor_t right_motor = { .count = 0,
                                   .speed = 0,
                                   .power = 0,
                                   .duty_limit = 100,
                                   .is_stalled = false,
                                   .direction = PUP_DIRECTION_CLOCKWISE };

static pup_motor_t left_motor = { .count = 0,
                                  .speed = 0,
                                  .power = 0,
                                  .duty_limit = 100,
                                  .is_stalled = false,
                                  .direction = PUP_DIRECTION_CLOCKWISE };

static pup_motor_t arm_motor = { .count = 0,
                                 .speed = 0,
                                 .power = 0,
                                 .duty_limit = 100,
                                 .is_stalled = false,
                                 .direction = PUP_DIRECTION_CLOCKWISE };

inline pup_motor_t* pup_motor_get_device(int port)
{
  if(port == PBIO_PORT_ID_A) {
    right_motor = { .count = 0,
                    .speed = 0,
                    .power = 0,
                    .duty_limit = 100,
                    .is_stalled = false,
                    .direction = PUP_DIRECTION_CLOCKWISE };
    return &right_motor;
  } else if(port == PBIO_PORT_ID_B) {
    left_motor = { .count = 0,
                   .speed = 0,
                   .power = 0,
                   .duty_limit = 100,
                   .is_stalled = false,
                   .direction = PUP_DIRECTION_CLOCKWISE };
    return &left_motor;
  } else if(port == PBIO_PORT_ID_C) {
    arm_motor = { .count = 0,
                  .speed = 0,
                  .power = 0,
                  .duty_limit = 100,
                  .is_stalled = false,
                  .direction = PUP_DIRECTION_CLOCKWISE };
    return &arm_motor;
  }
};

inline pbio_error_t pup_motor_setup(pup_motor_t* motor, pup_direction_t positive_direction,
                                    bool reset_count)
{
  if(!motor) {
    return PBIO_ERROR;
  }
  motor->direction = positive_direction;
  if(reset_count) {
    motor->count = 0;
  }
  return PBIO_SUCCESS;
};

inline pbio_error_t pup_motor_reset_count(pup_motor_t* motor)
{
  if(!motor) {
    return PBIO_ERROR;
  }
  motor->count = 0;
  return PBIO_SUCCESS;
};

inline int32_t pup_motor_get_count(pup_motor_t* motor)
{
  if(!motor) {
    return 0;
  }
  return motor->count;
};

inline int32_t pup_motor_get_speed(pup_motor_t* motor)
{
  if(!motor) {
    return 0;
  }
  return motor->speed;
};

inline pbio_error_t pup_motor_set_speed(pup_motor_t* motor, int speed)
{
  if(!motor) {
    return PBIO_ERROR;
  }
  motor->speed = speed;
  motor->count += speed;
  return PBIO_SUCCESS;
};

inline int32_t pup_motor_get_power(pup_motor_t* motor)
{
  if(!motor) {
    return 0;
  }
  return motor->power;
};

inline pbio_error_t pup_motor_set_power(pup_motor_t* motor, int power)
{
  if(!motor) {
    return PBIO_ERROR;
  }
  motor->power = power;
  motor->count += power;
  return PBIO_SUCCESS;
};

inline pbio_error_t pup_motor_stop(pup_motor_t* motor)
{
  if(!motor) {
    return PBIO_ERROR;
  }
  motor->power = 0;
  motor->speed = 0;
  return PBIO_SUCCESS;
};

inline pbio_error_t pup_motor_brake(pup_motor_t* motor)
{
  if(!motor) {
    return PBIO_ERROR;
  }
  motor->power = 0;
  motor->speed = 0;
  return PBIO_SUCCESS;
};

inline pbio_error_t pup_motor_hold(pup_motor_t* motor)
{
  if(!motor) {
    return PBIO_ERROR;
  }
  return PBIO_SUCCESS;
};

inline bool pup_motor_is_stalled(pup_motor_t* motor)
{
  if(!motor) {
    return false;
  }
  return motor->is_stalled;
};

inline int32_t pup_motor_set_duty_limit(pup_motor_t* motor, int duty_limit)
{
  if(!motor) {
    return 0;
  }
  int old_value = motor->duty_limit;
  motor->duty_limit = duty_limit;
  return old_value;
};

inline void pup_motor_restore_duty_limit(pup_motor_t* motor, int old_value)
{
  if(!motor) {
    return;
  }
  motor->duty_limit = old_value;
};

#endif  // _PUP_MOTOR_H_
