#pragma once
#include "esphome/core/helpers.h"
#include "esphome/core/log.h"
#include "esphome/core/component.h"
#include "esphome/components/uart/uart.h"

#include "typedefs.h"

#ifdef USE_SENSOR
#include "esphome/components/sensor/sensor.h"
#endif

#ifdef USE_BINARY_SENSOR
#include "esphome/components/binary_sensor/binary_sensor.h"
#endif

#ifdef USE_NUMBER
#include "esphome/components/number/number.h"
#include "number/entry_point.h"
#include "number/presence_region.h"
#endif

#ifdef USE_TEXT_SENSOR
#include "esphome/components/text_sensor/text_sensor.h"
#endif

#ifdef USE_SWITCH
#include "esphome/components/switch/switch.h"
#endif

#ifdef USE_BUTTON
#include "esphome/components/button/button.h"
#endif

#ifdef USE_SELECT
#include "esphome/components/select/select.h"
#endif

namespace esphome {
namespace ld2450 {

class EntryPoint;
class PresenceRegion;

class LD2450 : public uart::UARTDevice, public PollingComponent {
#ifdef USE_SENSOR
  SUB_SENSOR(position_x_0)
  SUB_SENSOR(position_y_0)
  SUB_SENSOR(speed_0)
  SUB_SENSOR(resolution_0)

  SUB_SENSOR(position_x_1)
  SUB_SENSOR(position_y_1)
  SUB_SENSOR(speed_1)
  SUB_SENSOR(resolution_1)

  SUB_SENSOR(position_x_2)
  SUB_SENSOR(position_y_2)
  SUB_SENSOR(speed_2)
  SUB_SENSOR(resolution_2)
  
  SUB_SENSOR(target_count)
#endif

#ifdef USE_NUMBER
  SUB_NUMBER(rotate)
  SUB_NUMBER(presence_timeout)
#endif

#ifdef USE_BINARY_SENSOR
  SUB_BINARY_SENSOR(target)
  SUB_BINARY_SENSOR(moving_target)
  SUB_BINARY_SENSOR(still_target)
#endif

#ifdef USE_TEXT_SENSOR
  SUB_TEXT_SENSOR(version)
  SUB_TEXT_SENSOR(mac)
#endif

#ifdef USE_SWITCH
  SUB_SWITCH(single_target)
  SUB_SWITCH(bluetooth)
#endif

#ifdef USE_BUTTON
  SUB_BUTTON(reset)
  SUB_BUTTON(reboot)
#endif

#ifdef USE_SELECT
  SUB_SELECT(baud_rate)
  SUB_SELECT(regions_type)
#endif

  public:
    void setup() override;
    void loop() override;
    void update() override;
    void dump_config() override;
    void read_all_info();
    void set_bluetooth(bool enable);
    void set_single_target(bool enable);
    void reboot_and_read();
    void restore_factory();
    void set_baud_rate(uint8_t state);
    void set_regions_type(uint8_t state);
    void add_entry_point(EntryPoint *entry_point);
    void add_presence_region(PresenceRegion *presence_region);

#ifdef USE_NUMBER
    void set_region_number(int region, int coord, number::Number *n);
    void set_region(uint8_t region);
    void set_rotate_number();
    void set_presence_timeout_number();
#endif

  private:
    enum Constants {
        ENABLE_CONFIG = 0xFF,
        DISABLE_CONFIG = 0xFE,
        SINGLE_TARGET = 0x80,
        MULTI_TARGET = 0x90,
        READ_FIRMWARE = 0xA0,
        SET_BAUDRATE = 0xA1,
        RESTORE_FACTORY = 0xA2,
        REBOOT = 0xA3,
        SET_BLUETOOTH = 0xA4,
        GET_MAC = 0xA5,
        GET_REGIONS = 0xC1,
        SET_REGIONS = 0xC2,
        FH_LAST = 0xFA,
        FE_LAST = 0x01,
        DE_LAST = 0xCC,
        DISABLE_REGIONS = 0x00,
        DETECT_REGIONS = 0x01,
        IGNORE_REGIONS = 0x02
    };   

    uint8_t FRAME_HEADER[4] = { 0xFD, 0xFC, 0xFB, FH_LAST };
    uint8_t FRAME_END[4] = { 0x04, 0x03, 0x02, FE_LAST };
    uint8_t DATA_HEADER[4] = { 0xAA, 0xFF, 0x03, 0x00 };
    uint8_t DATA_END[2] = { 0x55, DE_LAST };

    serial_buffer_struct serial_data;
    frame_start_struct response_frame_header;
    uint8_t response_buffer[256];
    data_packet_struct received_data;
    regions sensor_regions;
    std::string version_;
    std::string mac_;
    float rotate_angle = 0.0;
    coordinates person[3];
    coordinates person_before[3];
    float presence_timeout = 0.0;
    int32_t presence_millis[3];
    std::vector<EntryPoint *> entry_points;
    std::vector<PresenceRegion *> presence_regions;

#ifdef USE_NUMBER
    std::vector<std::vector<number::Number*>> region_numbers_ = std::vector<std::vector<number::Number*>>(3, std::vector<number::Number*>(4));;
#endif

    int16_t transform(uint16_t data);
    void report_position(void);
    coordinates rotate_coordinates(double x, double y, double angle);

    void send_command_(uint8_t command_str, const uint8_t *command_value, int command_value_len);
    void set_config_mode_(bool enable);
    void get_version_();
    void get_mac_();
    void set_single_target_();
    void set_multi_target_();
    void reboot_();
    void get_regions_();
};


}  // namespace ld2450
}  // namespace esphome
