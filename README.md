# Environmental Info Display

This project demonstrates how to monitor and display environmental information, including temperature, humidity, and time, using an STM32F4 microcontroller. The system employs a DHT11 sensor for temperature and humidity measurement and a DS3231 RTC for accurate timekeeping. Data is presented on a TM1637 display, with options for debugging through an OLED display.

## Features

- **Environmental Monitoring:** Measures and displays temperature and humidity using the DHT11 sensor.
- **Real-Time Clock (RTC):** Accurate timekeeping using the DS3231 RTC module.
- **Display System:** Uses a TM1637 7-segment display for real-time information output.
- **Calibration Functionality:** Allows for calibration of time settings using three buttons.
- **Debugging Interface:** Optional OLED display for debugging purposes.

## How It Works

### DHT11 Sensor Operation:

- The DHT11 sensor communicates with the STM32 using a custom One-Wire protocol.
- Temperature and humidity readings are updated every second for accuracy.

### RTC Functionality:

- The DS3231 RTC is used to maintain accurate time, which is displayed alongside environmental data.

### Display System:

- The TM1637 display is utilized to present temperature, humidity, and time information.
- The OLED display can be integrated for debugging purposes.

## Code Structure

- **main.c:**
  - Initializes the system components (DHT11 sensor, DS3231 RTC, TM1637 display).
  - Manages data acquisition and display updates in a loop.

- **dht11.c:**
  - Contains functions for initializing and communicating with the DHT11 sensor.
  - Reads temperature and humidity values.

- **ds3231.c:**
  - Interfaces with the DS3231 RTC for timekeeping functionality.

- **tm1637.c:**
  - Controls the TM1637 display for showing data.

## Prerequisites

- STM32F4 Microcontroller (e.g., STM32F429ZI)
- DHT11 Temperature and Humidity Sensor
- DS3231 Real-Time Clock Module
- TM1637 7-Segment Display
- Optional OLED Display for debugging
- STM32CubeIDE or IAR Embedded Workbench for compiling and flashing the project.

## How to Use

1. Clone the repository and open the project in your STM32 development environment.
   ```bash
   git clone https://github.com/yourusername/Environmental-Info-Display.git
   cd Environmental-Info-Display

This project is licensed under the MIT License - see the LICENSE file for details.

The project has been developed using the C programming language and was compiled using IAR Embedded Workbench, version 7.80. It is designed to run on an STM32 microcontroller, but can be adapted to other platforms with minor modifications.

Please note that only the core project files (dht11.c, dht11.h, tm1637.c, tm1637.h, ds3231.h, ds3231.c and main.c) have been provided. You will need to implement other necessary configurations, such as IO initialization and RCC enabling, according to your own compiler and library setup. Ensure that the hardware-specific settings are adjusted for your environment.

The project also includes the design of the electronic circuit schematic, PCB drawing, outlining the components and their connections. 

Demonstration Video Link: https://www.youtube.com/watch?v=_ADOjVoNexo&ab_channel=MucahitEmbeddedSystems