# Tiresias Firmware

![USP](https://img.shields.io/badge/research-EESC_USP-blue?style=for-the-badge)
![License](https://img.shields.io/badge/License-MIT-green?style=for-the-badge)
![GitHub contributors](https://img.shields.io/github/contributors/felipepimentab/tiresias-fw?style=for-the-badge)
![GitHub last commit](https://img.shields.io/github/last-commit/felipepimentab/tiresias-fw?style=for-the-badge)

Source code and associated files for the firmware used in the Tiresias project

## ☕️ Introduction

Welcome to the **Tiresias Firmware** repository! This repository contains the source code and associated files for the firmware used in the Tiresias project, a Brazilian research initiative conducted at [EESC-USP](https://www.eesc.usp.br/) by MSc students. The goal of the Tiresias project is to develop a free and open-source national technology for hearing aid applications using off-the-shelf components.

The firmware is designed for the Nordic Semiconductor's **nRF5340 SoC**, running the **Zephyr RTOS**, and integrating the Analog Devices' **ADAU1787 audio codec**.

> Note: This project is currently designed to integrate the ADAU1701 audio codec as a step of the development proccess aiming to reach a _proof of concept_.

## 📚 Project Overview

### About the Tiresias Project

The Tiresias project is an innovative effort to create an open-source and accessible hearing aid solution tailored to the needs of the Brazilian population. Our aim is to reduce dependency on expensive proprietary technologies by leveraging widely available components and state-of-the-art design practices.

### Key Features

- **Connectivity**: Utilizes the Nordic nRF5340 SoC for Bluetooth Low Energy (BLE) communication, ensuring seamless wireless connectivity.
- **Audio Processing**: Integrates the Analog Devices ADAU1787 Audio Codec for high-quality audio processing, essential for effective hearing aid functionality.
- **Open-Source**: All code and design files are open-source, allowing for community collaboration and transparency in the development process.

## 🗂️ Repository Structure

The repository is structured as follows:

```plaintext
tiresias-fw/
├── docs/                   # Documentation files
├── src/                    # Source code for the firmware
│   ├── main.c              # Main application code
│   ├── drivers/            # Drivers and utilities
│   ├── modules/            # Modules and utilities
│   └── SigmaStudioFiles/   # Automatically generated files from the SigmaStudio software
├── CMakeLists.txt          # CMake build system configuration
└── README.md               # This README file
```

## 🛠️ Getting Started

### Hardware overview

The project uses a [nRF5340 SoC](https://www.nordicsemi.com/Products/nRF5340) from [Nordic Semiconductors](https://www.nordicsemi.com). It contains an application proccessor for handling the main functions and a dedicated network processor for handling Bluetooth Low Energy (BLE) connectivity.

For digital audio processing, the [ADAU1787 Audio Codec](https://www.analog.com/en/products/adau1787.html) from [Analog Devices](https://www.analog.com/en/index.html) provides speed and efficiency with its SigmaDSP audio processing core and FastDSP audio processing engine.

The ADAU1787 is controlled via I2C by the nRF5340, which also handles BLE connection for wirelles operation.

### Firmware overview

> TODO: finish the firmware overview section

The nRF5340 SoC runs the Zephyr RTOS...

The program for the ADAU1787 is built using the SigmaStudio software provided by Analog Devices. It allows the DSP to be graphically programmed and have its program exported as a header file for loading via I2C.

## 📝 License

This project is licensed under the MIT License - see the LICENSE file for details.

## 📮 Acknowledgments

- EESC-USP: For providing the infrastructure and support for this research.
- Nordic Semiconductor: For providing the nRF5340 Audio Development Kits (DK) used in this project and extensive support.
- Analog Devices: For providing the ADAU1787 codec used in this project and extensive support.

## 👥 Contributors

<table>
  <tr>
    <td align="center">
      <a href="https://www.linkedin.com/in/felipepimentab/">
        <img src="https://media.licdn.com/dms/image/v2/D4D03AQFyoYsj9tdKNQ/profile-displayphoto-shrink_800_800/profile-displayphoto-shrink_800_800/0/1703206014313?e=1730937600&v=beta&t=18_O5J1wTH-HddT_AuEWWBmmE-W6BBoCV4ESLxacxYU" width="100px;" alt="Felipe Pimenta Bernardo on LinkedIn"/><br>
        <sub>
          <b>Felipe Pimenta Bernardo</b>
        </sub>
      </a>
    </td>
    <td align="center">
      <a href="https://www.linkedin.com/in/joaocolombari/">
        <img src="https://media.licdn.com/dms/image/v2/D4D03AQFsZ0xGEuZxrg/profile-displayphoto-shrink_800_800/profile-displayphoto-shrink_800_800/0/1692278514805?e=1730937600&v=beta&t=4HyZxSg3sPDtpvB7AkE4bi-QBfpsZywh1Njb9Q2T2sY" width="100px;" alt="João Colombari on LinkedIn"/><br>
        <sub>
          <b>João Victor Colombari</b>
        </sub>
      </a>
    </td>
  </tr>
</table>

## 🤝 Contributing

Contributions are welcome! Fork the repository, create a feature branch, make your changes, and open a pull request.

## 📩 Contact

For any inquiries or further information, please contact us on LinkedIn by clicking on our profiles above.

Thank you for your interest in the Tiresias project!
