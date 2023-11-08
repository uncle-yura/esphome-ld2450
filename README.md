# esphome-ld2450

## Features
1. Obtaining the coordinates of three persons
1. Ability to rotate coordinates relative to zero
1. Settings of zones
1. Bluetooth control
1. Reading sensor data
1. Data update frequency is determined by update_interval

## Supported Microcontrollers

Tested with:
- ESP8266 (board Wemos D1 mini)

## Usage
### Step 1: Build a control circuit.
Connect the 5V power supply and the UART from the microcontroller to the sensor. Be careful, the color of the wires may differ.

### Step 2: Configure your ESPHome device with YAML
See the file `example_ld2450.yaml`
