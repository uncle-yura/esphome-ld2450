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
```
esphome:
  name: hlk-presense
  friendly_name: hlk-presense

external_components:
  - source:
      type: git
      url: https://github.com/uncle-yura/esphome-ld2450
      ref: master
    components: [ ld2450 ]

esp8266:
  board: d1_mini

# Enable logging
logger:
  baud_rate: 0

# Enable Home Assistant API
api:
  encryption:
    key: "your-encryption-key"

ota:
  password: "your-ota-password"

wifi:
  ssid: !secret wifi_ssid
  password: !secret wifi_password

  # Enable fallback hotspot (captive portal) in case wifi connection fails
  ap:
    ssid: "your-sensor-hotspot"
    password: "your-password"

captive_portal:   

uart:
  id: uart_bus
  tx_pin: 
    number: GPIO1
  rx_pin: 
    number: GPIO3
  baud_rate: 256000
  parity: NONE
  stop_bits: 1

ld2450:
  uart_id: uart_bus
  update_interval: 1s

binary_sensor:
- platform: ld2450
  has_target:
    name: Presence
  has_moving_target:
    name: Moving Target
  has_still_target:
    name: Still Target

text_sensor:
  - platform: ld2450
    version:
      name: "FW"
    mac_address:
      name: "MAC address"

button:
  - platform: restart
    name: "ESP Restart"
  - platform: ld2450
    factory_reset:
      name: "Factory reset"
    reboot:
      name: "Reboot"

switch:
  - platform: ld2450
    single_target:
      name: "Single target"
    bluetooth:
      name: "Bluetooth"

number:
- platform: ld2450
  rotate:
    restore_value: true
    initial_value: 0
    name: "Rotate angle"
  region_0:
    x0:
      name: R0X0
    y0:
      name: R0Y0
    x1:
      name: R0X1
    y1:
      name: R0Y1

  region_1:
    x0:
      name: R1X0
    y0:
      name: R1Y0
    x1:
      name: R1X1
    y1:
      name: R1Y1

  region_2:
    x0:
      name: R2X0
    y0:
      name: R2Y0
    x1:
      name: R2X1
    y1:
      name: R2Y1

select:
  - platform: ld2450
    baud_rate:
      name: "Baud rate"
    regions_type:
      name: "Regions type"

sensor:
- platform: ld2450
  target_count:
    name: Target count

  person_0:
    position_x:
      name: "P0X"  

    position_y:
      name: "P0Y"  

    speed:
      name: "S0"  

    resolution:
      name: "R0"  

  person_1:
    position_x:
      name: "P1X"  

    position_y:
      name: "P1Y"  

    speed:
      name: "S1"  

    resolution:
      name: "R1"  

  person_2:
    position_x:
      name: "P2X"  

    position_y:
      name: "P2Y"  

    speed:
      name: "S2"  

    resolution:
      name: "R2"
```