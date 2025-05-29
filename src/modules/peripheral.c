#include "peripheral.h"
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(peripheral_module, LOG_LEVEL_INF);

/* Button info: Label and pin mapping from devicetree aliases */
#define BTN_NODE(idx) DT_ALIAS(sw##idx)
#define BTN_EXISTS(idx) DT_NODE_HAS_STATUS(BTN_NODE(idx), okay)

#define BTN_COUNT 4

/* Static button config */
struct button_config {
  const struct gpio_dt_spec spec;
  struct gpio_callback callback;
  uint8_t id;
};

static struct button_config buttons[BTN_COUNT] = {
#if BTN_EXISTS(0)
  { .spec = GPIO_DT_SPEC_GET(BTN_NODE(0), gpios), .id = 1 },
#endif
#if BTN_EXISTS(1)
  { .spec = GPIO_DT_SPEC_GET(BTN_NODE(1), gpios), .id = 2 },
#endif
#if BTN_EXISTS(2)
  { .spec = GPIO_DT_SPEC_GET(BTN_NODE(2), gpios), .id = 3 },
#endif
#if BTN_EXISTS(3)
  { .spec = GPIO_DT_SPEC_GET(BTN_NODE(3), gpios), .id = 4 },
#endif
};

/* Interrupt handler */
static void button_pressed_isr(const struct device* dev, struct gpio_callback* cb, uint32_t pins)
{
  struct button_config* btn = CONTAINER_OF(cb, struct button_config, callback);
  LOG_INF("Button %d pressed", btn->id);
}

int peripheral_init(void)
{
  int ret;

  for (int i = 0; i < BTN_COUNT; i++) {
    const struct button_config* btn = &buttons[i];

    if (!device_is_ready(btn->spec.port)) {
      LOG_ERR("GPIO device not ready for button %d", btn->id);
      return -ENODEV;
    }

    ret = gpio_pin_configure_dt(&btn->spec, GPIO_INPUT | GPIO_PULL_UP);
    if (ret < 0) {
      LOG_ERR("Failed to configure button %d", btn->id);
      return ret;
    }

    ret = gpio_pin_interrupt_configure_dt(&btn->spec, GPIO_INT_EDGE_TO_ACTIVE);
    if (ret < 0) {
      LOG_ERR("Failed to enable interrupt for button %d", btn->id);
      return ret;
    }

    gpio_init_callback(&buttons[i].callback, button_pressed_isr, BIT(btn->spec.pin));
    gpio_add_callback(btn->spec.port, &buttons[i].callback);
  }

  LOG_INF("Peripheral buttons initialized.");
  return 0;
}