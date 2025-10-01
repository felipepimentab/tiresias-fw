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

### Initialization

Initialize a new workspace for the Tiresias Firmware project:

```sh
west init -m https://github.com/felipepimentab/tiresias-fw --mr main tiresias-workspace
```

Update the project's modules:

```sh
cd tiresias-workspace
west update
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
        <img src="https://github.com/felipepimentab.png" width="100px;" alt="Felipe Pimenta Bernardo on GitHub"/><br>
        <sub>
          <b>Felipe Pimenta Bernardo</b>
        </sub>
      </a>
    </td>
    <td align="center">
      <a href="https://www.linkedin.com/in/joaocolombari/">
        <img src="https://github.com/joaocolombari.png" width="100px;" alt="João Colombari on GitHub"/><br>
        <sub>
          <b>João Victor Colombari</b>
        </sub>
      </a>
    </td>
    <td align="center">
      <a href="https://www.linkedin.com/in/bruno-henriques-321b16121/">
        <img src="https://github.com/brunochenriques.png" width="100px;" alt="Bruno Henriques on GitHub"/><br>
        <sub>
          <b>Bruno Henriques</b>
        </sub>
      </a>
    </td>
  </tr>
</table>

## 🤝 Contributing

Contributions are welcome! Fork the repository, create a feature branch, make your changes, and open a pull request.

Please follow our versioning guidelines when contributing to the project.

## 📩 Contact

For any inquiries or further information, please contact us on LinkedIn by clicking on our profiles above.

Thank you for your interest in the Tiresias project!
