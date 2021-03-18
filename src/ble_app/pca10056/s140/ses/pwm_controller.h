#ifndef PWM_CONTROLLER
#define PWM_CONTROLLER

#include <stdio.h>
#include <string.h>

#include "nrf_drv_pwm.h"
#include "nrfx_pwm.h"
#include "app_util_platform.h"
#include "app_error.h"
#include "boards.h"
#include "bsp.h"
#include "app_timer.h"
#include "nrf_drv_clock.h"
#include "nrf_gpio.h"

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

typedef enum
{
  PWM_LINEAR1,
  PWM_LINEAR2,
  PWM_LINEAR3,
  PWM_LEFT,
  PWM_RIGHT,
  PWM_LATERAL_VARIATION,
  PWM_ROLLING_HILLS  
} pwm_setting_t;

static pwm_setting_t currentPwmSetting;

/**@brief Function for initializing the application for PWM support
 *
 * @details Initializes PWM settings.
 *
 * @note 
 *
 */
void initPwm(pwm_setting_t setting);

void pwmHandler();

void completeSequence(nrf_pwm_values_common_t * sequence, const uint16_t value);

#endif