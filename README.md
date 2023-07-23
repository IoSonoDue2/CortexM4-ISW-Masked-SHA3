# README

## Project Overview
This repository contains the codebase for a project involving the implementation of the Ishai-Sahai-Wagner (ISW) protection scheme to safeguard the Secure Hash Algorithm 3 (SHA-3) against Differential Power Analysis (DPA) attacks on ARM Cortex M4 microcontrollers. The implementation also includes a shuffling technique to enhance the robustness of the ISW scheme.

This project was supervised by [Alessandro Barenghi](https://github.com/alexrow).

## Repo Structure
This repository is organized as follows:

- `x86_tests/`: Contains the source code for the x86 implementation of the algorithm. This allows for performing tests without the need for cross-compiling.

- `STMF4DISCOVERY/`: Contains the source code for the implementation designed specifically for the STMF4 Discovery microcontroller.

### Building and Testing the Project

#### x86 Implementation
1. Navigate to the `x86_tests/` directory.
2. Compile and build the project according to your environment and the build instructions in the project files.
3. Run tests as needed.

#### STM32F4 Discovery Microcontroller Implementation
For this, i recommend using PlatformIO with the following `platformio.ini` settings:

```
[env:disco_f407vg]
platform = ststm32
board = disco_f407vg
framework = libopencm3
upload_protocol = mbed
```

1. Navigate to the `STMF4DISCOVERY/` directory.
2. Compile and build the project using PlatformIO.
3. Flash the compiled binary to your STM32F4 Discovery board using the PlatformIO upload feature.
4. Run tests on the hardware as needed.

## Contributions
Feel free to contribute to this project by opening issues or providing pull requests. Please ensure that all contributions align with the existing coding style for consistency.

## License
This project is licensed under the terms of the MIT license.

