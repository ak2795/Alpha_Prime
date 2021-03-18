#include "pwm_controller.h"

static nrf_drv_pwm_t m_pwm0 = NRFX_PWM_INSTANCE(0);

void initPwm(pwm_setting_t setting)
{
  NRF_LOG_INFO("Initializing PWM Module.");
  nrf_gpio_cfg_output(7);
  nrf_gpio_cfg_output(8);

  // Initialize the application to Linear 1 setting
  currentPwmSetting = setting;
  pwmHandler();
}

void completeSequence(nrf_pwm_values_common_t * sequence, const uint16_t value)
{
  for (int i = 0; i < 100; i++)
  {
    sequence[i] = value;
  }
}

void pwmHandler()
{
    nrf_drv_pwm_uninit(&m_pwm0);

    enum { // [local constants]
        TOP        = 10000,
        MID        = 7500,
        LOW        = 2000,
        STEP_COUNT = 100
    };

    nrf_drv_pwm_config_t const config0 =
    {
        .output_pins =
        {
            LED_3,                                     // channel 0
            LED_4,                                     // channel 1
            ARDUINO_13_PIN | NRF_DRV_PWM_PIN_INVERTED, // channel 2
            ARDUINO_12_PIN | NRF_DRV_PWM_PIN_INVERTED  // channel 3
        },
        .irq_priority = APP_IRQ_PRIORITY_LOWEST,
        .base_clock   = NRF_PWM_CLK_500kHz,
        .count_mode   = NRF_PWM_MODE_UP,
        .top_value    = TOP,
        .load_mode    = NRF_PWM_LOAD_COMMON,
        .step_mode    = NRF_PWM_STEP_AUTO
    };
    APP_ERROR_CHECK(nrf_drv_pwm_init(&m_pwm0, &config0, NULL));

    // This array cannot be allocated on stack (hence "static") and it must
    // be in RAM (hence no "const", though its content is not changed).
    static nrf_pwm_values_common_t /*const*/ seq0_values[STEP_COUNT];
    uint16_t value;
    uint16_t step  = TOP / STEP_COUNT;
    uint8_t  i;

    switch (currentPwmSetting)
    {
      case PWM_LINEAR1:
        completeSequence((nrf_pwm_values_common_t *) &seq0_values, LOW);
        break;
      case PWM_LINEAR2:
        completeSequence((nrf_pwm_values_common_t *) &seq0_values, MID);
        break;
      case PWM_LINEAR3:
        completeSequence((nrf_pwm_values_common_t *) &seq0_values, TOP);
        break;
      case PWM_ROLLING_HILLS:
        value = 0;
        // Increase
        for (i = 0; i < STEP_COUNT / 2; i++)
        {
            value         += step;
            seq0_values[i] = value;
        }
        // Decrease
        for (; i < STEP_COUNT; i++)
        {
            value         -= step;
            seq0_values[i] = value;
        }
        break;
      default:
        break;
    }
    
    nrf_pwm_sequence_t const seq0 =
    {
        .values.p_common = seq0_values,
        .length          = NRF_PWM_VALUES_LENGTH(seq0_values),
        .repeats         = 1,
        .end_delay       = 0
    };

    (void)nrf_drv_pwm_simple_playback(&m_pwm0, &seq0, 1,
                                      NRF_DRV_PWM_FLAG_LOOP);
}