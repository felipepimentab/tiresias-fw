#include <zephyr/kernel.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/shell/shell.h>
#include <zephyr/zbus/zbus.h>\


#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(Main_app, LOG_LEVEL_INF);

int main(void) {
	// Initialize I2C
	i2c_init();

	// Initialize ADAU1701
	adau1701_init();

	// Load DSP program and parameters
	uint8_t dsp_program[] = { /* Your DSP program data here */ };
	uint8_t dsp_parameters[] = { /* Your DSP parameters here */ };

	adau1701_load_program(dsp_program, sizeof(dsp_program));
	adau1701_load_parameters(dsp_parameters, sizeof(dsp_parameters));

	// Set volume and mute
	adau1701_set_volume(128); // Example volume level
	adau1701_mute(false);      // Unmute

	while (1) {
		// Main loop
	}
}