# Tiresias Firmware

![USP](https://img.shields.io/badge/research-EESC_USP-blue?style=for-the-badge)
![License](https://img.shields.io/badge/License-MIT-green?style=for-the-badge)
![Version](https://img.shields.io/badge/Version-0.1.0-blue?style=for-the-badge)
![GitHub contributors](https://img.shields.io/github/contributors/felipepimentab/tiresias-fw?style=for-the-badge)
![GitHub last commit](https://img.shields.io/github/last-commit/felipepimentab/tiresias-fw?style=for-the-badge)

Source code and associated files for the firmware used in the Tiresias project

## ☕️ Introduction

Welcome to the **Tiresias Firmware** repository! This repository contains the source code and associated files for the firmware used in the [Tiresias Project](https://tiresias-docs.vercel.app), a Brazilian research initiative conducted at [EESC-USP](https://www.eesc.usp.br/) by MSc students. The goal of the Tiresias Project is to develop a free and open-source national technology for hearing aid applications using off-the-shelf components.

The firmware is designed for the Nordic Semiconductor's [nRF5340 SoC](https://www.nordicsemi.com/Products/nRF5340), running the [Zephyr RTOS](https://www.zephyrproject.org), and integrating the Analog Devices' [ADAU1787 audio codec](https://www.analog.com/en/products/adau1787.html).

## 📚 Project Overview

### About the Tiresias Project

The Tiresias Project is an innovative effort to create an open-source and accessible hearing aid solution tailored to the needs of the Brazilian population. Our aim is to reduce dependency on expensive proprietary technologies by leveraging widely available components and state-of-the-art design practices.

### Key Features

- **Connectivity**: Utilizes the Nordic nRF5340 SoC for Bluetooth Low Energy (BLE) communication, ensuring seamless wireless connectivity.
- **Audio Processing**: Integrates the Analog Devices ADAU1787 Audio Codec for high-quality audio processing, essential for effective hearing aid functionality.
- **Open-Source**: All code and design files are open-source, allowing for community collaboration and transparency in the development process.

## 🛠️ Getting Started

### Hardware overview

The project uses a [nRF5340 SoC](https://www.nordicsemi.com/Products/nRF5340) from [Nordic Semiconductors](https://www.nordicsemi.com). It contains an application proccessor for handling the main functions and a dedicated network processor for handling Bluetooth Low Energy (BLE) connectivity.

For digital audio processing, the [ADAU1787 Audio Codec](https://www.analog.com/en/products/adau1787.html) from [Analog Devices](https://www.analog.com/en/index.html) provides speed and efficiency with its SigmaDSP audio processing core and FastDSP audio processing engine.

The ADAU1787 is controlled via I2C by the nRF5340, which also handles BLE connection for wirelles operation.

### Firmware overview

```plaintext
Controller (Application)
│
├── Audio Codec (Service)
│   ├── adau1787 (Module)
│   │   └── i2c (Zephyr Driver)
│   └── i2s_control (Module)
│       └── i2s (Zephyr Driver)
│
├── Bluetooth (Service)
│   └── ble_profile (Module)
│       └── ble (Zephyr Driver)
│
├── peripheral (Module)
│   └── gpio (Zephyr Driver)
│
└── storage (Module)
    └── nvs (Zephyr Driver)
```

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
        <img src="https://media.licdn.com/dms/image/v2/D4D03AQFyoYsj9tdKNQ/profile-displayphoto-shrink_800_800/profile-displayphoto-shrink_800_800/0/1703206014313?e=1756339200&v=beta&t=gYEPW4J-IrkjDcsqruskhAkUTVabUPuZ1pHwwJ6uOlA" width="100px;" alt="Felipe Pimenta Bernardo on LinkedIn"/><br>
        <sub>
          <b>Felipe Pimenta Bernardo</b>
        </sub>
      </a>
    </td>
    <td align="center">
      <a href="https://www.linkedin.com/in/joaocolombari/">
        <img src="https://media.licdn.com/dms/image/v2/D4D03AQFD-hNZ1GKuXA/profile-displayphoto-shrink_800_800/B4DZcw9ykIGUAc-/0/1748873194277?e=1756339200&v=beta&t=uSvDTtqZZkaMX_0TsrSo-0BUP34ut-sS3rHq8DOgfv8" width="100px;" alt="João Colombari on LinkedIn"/><br>
        <sub>
          <b>João Victor Colombari</b>
        </sub>
      </a>
    </td>
    <td align="center">
      <a href="https://www.linkedin.com/in/bruno-henriques-321b16121/">
        <img src="https://media.licdn.com/dms/image/v2/C5103AQEXMdFo89PoXQ/profile-displayphoto-shrink_800_800/profile-displayphoto-shrink_800_800/0/1517004559859?e=1756339200&v=beta&t=kJyiXqvCFNW-FvnJttJnG9dII_PskhOtZMhaRQ7jNV4" width="100px;" alt="Bruno Henriques on LinkedIn"/><br>
        <sub>
          <b>Bruno Henriques</b>
        </sub>
      </a>
    </td>
  </tr>
</table>

## ✅ Tasks

The following tasks show our progress and future development plans:

### Completed

- [x] **Implement Core Architecture**: Set up the foundational system architecture
  - [x] Create modular thread-based design
  - [x] Implement message passing between modules
  - [x] Set up build system and project structure

- [x] **Implement Storage Module**: Create persistent storage for user settings
  - [x] Implement NVS-based storage service
  - [x] Add volume persistence functionality
  - [x] Create thread-safe asynchronous API

- [x] **Implement Peripheral Module**: Add support for buttons and LEDs
  - [x] Create button event handling with callbacks
  - [x] Implement LED control functionality
  - [x] Add thread-safe API for peripheral access

### In Progress

- [ ] **Implement ADAU1787 Driver**: Create driver for the audio codec
  - [x] Implement I2C communication with the codec
  - [X] Add startup (boot) sequence
  - [x] Add register read/write functionality
  - [ ] Implement Safeload write operations
  - [ ] Implement SigmaDSP parameter loading

- [ ] **Implement I2S Control**: Set up audio data transfer
  - [ ] Configure I2S interface for stereo audio
  - [ ] Implement block-based audio transfer
  - [ ] Set up proper audio parameters (44.1kHz, 16-bit)

- [ ] **Implement Bluetooth Service**: Add BLE functionality for wireless control of the hearing aid
  - [ ] Configure Bluetooth in prj.conf
  - [ ] Create BLE service module with custom hearing aid profile
  - [ ] Implement secure pairing and bonding
  - [ ] Integrate with controller for command routing

- [ ] **Complete Audio Codec Functionality**: Finish the implementation of core audio features
  - [ ] Implement volume control via I2C
  - [ ] Add mute/unmute functionality
  - [ ] Create different audio processing programs/presets

- [ ] **Add Power Management**: Optimize for battery-powered operation
  - [ ] Implement sleep modes when audio processing is not needed
  - [ ] Add battery monitoring and low-battery alerts
  - [ ] Optimize BLE connection parameters for power efficiency

- [ ] **Enhance User Interaction**: Improve the user experience
  - [ ] Map button functions to hearing aid-specific controls
  - [ ] Add LED indicators for device status
  - [ ] Implement user feedback mechanisms

- [ ] **Implement Adaptive Features**: Add environment-aware functionality
  - [ ] Develop noise detection and classification
  - [ ] Create automatic program selection based on environment
  - [ ] Implement adaptive gain control

## 📦 Versioning and Releases

This project follows [Semantic Versioning](https://semver.org/) with the format `MAJOR.MINOR.PATCH`:

- **MAJOR**: Incremented for incompatible API changes
- **MINOR**: Incremented for backward-compatible functionality additions
- **PATCH**: Incremented for backward-compatible bug fixes

For more details on our versioning and release process, see [VERSIONING.md](VERSIONING.md).

All notable changes to the project are documented in the [CHANGELOG.md](CHANGELOG.md) file.

## 🤝 Contributing

Contributions are welcome! Fork the repository, create a feature branch, make your changes, and open a pull request.

Please follow our versioning guidelines when contributing to the project.

## 📩 Contact

For any inquiries or further information, please contact us on LinkedIn by clicking on our profiles above.

Thank you for your interest in the Tiresias project!
