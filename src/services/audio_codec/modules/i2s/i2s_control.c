#include "i2s_control.h"
#include <nrfx_clock.h>
#include <nrfx_i2s.h>
#include <zephyr/device.h>
#include <zephyr/drivers/pinctrl.h>
#include <zephyr/kernel.h>

#define I2S_NL DT_NODELABEL(i2s0)

PINCTRL_DT_DEFINE(I2S_NL);

/**< MCLK/LRCLK ratio -> LRCLOCK (sample rate) = 48kHz */
#define AUDIO_RATIO NRF_I2S_RATIO_128X

static nrfx_i2s_t i2s_inst = NRFX_I2S_INSTANCE(0);

static nrfx_i2s_config_t cfg = {
  .skip_gpio_cfg = true,
  .skip_psel_cfg = true,
  .irq_priority = DT_IRQ(I2S_NL, priority),
  .mode = NRF_I2S_MODE_MASTER,
  .format = NRF_I2S_FORMAT_I2S,
  .alignment = NRF_I2S_ALIGN_LEFT,
  .ratio = AUDIO_RATIO,
  .mck_setup = 0x66666000,
  .sample_width = NRF_I2S_SWIDTH_16BIT,
  .channels = NRF_I2S_CHANNELS_STEREO,
  .clksrc = NRF_I2S_CLKSRC_ACLK,
  .enable_bypass = false,
};

static void i2s_comp_handler(nrfx_i2s_buffers_t const* released_bufs, uint32_t status)
{
  // Do nothing
}

int audio_i2s_init(void)
{
  nrfx_err_t ret = 0;

  nrfx_clock_hfclkaudio_config_set(HFCLKAUDIO_12_165_MHZ);

  NRF_CLOCK->TASKS_HFCLKAUDIOSTART = 1;

  while (!NRF_CLOCK_EVENT_HFCLKAUDIOSTARTED) {
    k_sleep(K_MSEC(1));
  }

  ret = pinctrl_apply_state(PINCTRL_DT_DEV_CONFIG_GET(I2S_NL), PINCTRL_STATE_DEFAULT);
  __ASSERT_NO_MSG(ret == 0);

  IRQ_CONNECT(DT_IRQN(I2S_NL), DT_IRQ(I2S_NL, priority), nrfx_isr, nrfx_i2s_0_irq_handler, 0);
  irq_enable(DT_IRQN(I2S_NL));

  ret = nrfx_i2s_init(&i2s_inst, &cfg, i2s_comp_handler);
  __ASSERT_NO_MSG(ret == NRFX_SUCCESS);

  if (ret == NRFX_SUCCESS) {
    return 0;
  }

  return ret;
}