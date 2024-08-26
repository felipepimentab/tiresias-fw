#include "nrf_drv_twi.h" // Include Nordic TWI/I2C driver header

#define ADAU1701_I2C_ADDRESS 0x34 // I2C address for ADAU1701

nrf_drv_twi_t m_twi = NRF_DRV_TWI_INSTANCE(0); // TWI instance

// Function to initialize I2C
void i2c_init(void) {
    nrf_drv_twi_config_t const config = {
       .scl                = SCL_PIN,    // Set your SCL pin
       .sda                = SDA_PIN,    // Set your SDA pin
       .frequency          = NRF_TWI_FREQ_400K, // 400 kHz I2C frequency
       .interrupt_priority = APP_IRQ_PRIORITY_HIGH,
       .clear_bus_init     = false
    };

    nrf_drv_twi_init(&m_twi, &config, NULL, NULL);
    nrf_drv_twi_enable(&m_twi);
}

// Function to write to an ADAU1701 register
ret_code_t adau1701_write_register(uint8_t reg_address, uint8_t* data, size_t length) {
    uint8_t buffer[length + 1];
    buffer[0] = reg_address;
    memcpy(&buffer[1], data, length);

    return nrf_drv_twi_tx(&m_twi, ADAU1701_I2C_ADDRESS, buffer, length + 1, false);
}

// Function to read from an ADAU1701 register
ret_code_t adau1701_read_register(uint8_t reg_address, uint8_t* data, size_t length) {
    ret_code_t err_code;

    err_code = nrf_drv_twi_tx(&m_twi, ADAU1701_I2C_ADDRESS, &reg_address, 1, true);
    if (err_code != NRF_SUCCESS) {
        return err_code;
    }

    return nrf_drv_twi_rx(&m_twi, ADAU1701_I2C_ADDRESS, data, length);
}

void adau1701_init(void) {
    /* When the SELFBOOT pin is low at power-up, the part
    defaults to I2C mode */ 

    // Example PLL Control register configuration
    uint8_t pll_ctrl_data[6] = {0x00, 0x7D, 0x00, 0x00, 0x00, 0x01}; // Set appropriate values based on your system

    // Write to PLL Control Register (0x4002)
    adau1701_write_register(0x4002, pll_ctrl_data, sizeof(pll_ctrl_data));
    
    // Example: Set Core Clock Enable (0x081C)
    uint8_t core_clock_enable = 0x01;
    adau1701_write_register(0x081C, &core_clock_enable, 1);

    // Other initialization steps can be added here
}

void adau1701_load_program(uint8_t* program_data, size_t program_size) {
    // Write the DSP program to ADAU1701
    adau1701_write_register(PROGRAM_MEMORY_START_ADDRESS, program_data, program_size);
}

void adau1701_load_parameters(uint8_t* param_data, size_t param_size) {
    // Write the DSP parameters to ADAU1701
    adau1701_write_register(PARAMETER_MEMORY_START_ADDRESS, param_data, param_size);
}

void adau1701_set_volume(uint8_t volume) {
    uint8_t volume_data = volume; // Assuming 8-bit volume control
    adau1701_write_register(VOLUME_REGISTER_ADDRESS, &volume_data, 1);
}

void adau1701_mute(bool mute) {
    uint8_t mute_data = mute ? 0x01 : 0x00;
    adau1701_write_register(MUTE_REGISTER_ADDRESS, &mute_data, 1);
}