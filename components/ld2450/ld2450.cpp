#include "ld2450.h"

namespace esphome {
namespace ld2450 {

static const char *TAG = "ld2450";

#define PERSON_PUBLISH_(name, person, val) \
    sensor::Sensor *name##_##person; \
    name##_##person = this->name##_##person##_sensor_; \
    if (name##_##person != nullptr) name##_##person->publish_state(val);
#define PERSON_PUBLISH(name, person, val) PERSON_PUBLISH_(name, person, val)

#define highbyte(val) (uint8_t)((val) >> 8)
#define lowbyte(val) (uint8_t)((val) &0xff)

void LD2450::setup() {
    this->set_timeout(1000, [this]() { this->read_all_info(); });
}

void LD2450::update() {
    report_position();

#ifdef USE_TEXT_SENSOR
    if (this->mac_text_sensor_ != nullptr && this->mac_text_sensor_->state != this->mac_) {
        this->mac_text_sensor_->publish_state(this->mac_);
    }

    if (this->version_text_sensor_ != nullptr && this->version_text_sensor_->state != this->version_) {
        this->version_text_sensor_->publish_state(this->version_);
    }
#endif
}

void LD2450::loop() {
    while (available()) {
        char c = read();
        serial_data.buffer[serial_data.size] = c;
        serial_data.size ++;

        switch(c) {
            case DE_LAST: {
                uint8_t de_size = sizeof(DATA_END);
                uint8_t packet_size = sizeof(data_packet_struct);

                if(serial_data.size >= packet_size && memcmp(serial_data.buffer+serial_data.size-de_size, DATA_END, de_size) == 0) {
                    memcpy(&received_data, serial_data.buffer+serial_data.size-packet_size, packet_size);
                    serial_data.size = 0;
                }
                break;
            }
            case FH_LAST: {
                uint8_t fh_size = sizeof(FRAME_HEADER);
                if(serial_data.size >= fh_size && memcmp(serial_data.buffer+serial_data.size-fh_size, FRAME_HEADER, fh_size) == 0) {
                    serial_data.frame_start = serial_data.size - fh_size;
                }
                break;
            }
            case FE_LAST: {
                uint8_t fe_size = sizeof(FRAME_END);
                uint8_t min_packet_size = sizeof(FRAME_HEADER) + fe_size + 6;

                if(serial_data.size >= min_packet_size && memcmp(serial_data.buffer+serial_data.size-fe_size, FRAME_END, fe_size) == 0) {
                    uint8_t frame_size = sizeof(response_frame_header);
                    memcpy(&response_frame_header, serial_data.buffer+serial_data.frame_start, frame_size);

                    if(response_frame_header.command_ack) {
                        if(response_frame_header.command == GET_REGIONS) {
                            uint8_t sensor_regions_size = sizeof(sensor_regions);
                            memcpy(&sensor_regions, serial_data.buffer+serial_data.frame_start+frame_size, sensor_regions_size);
                        } else {
                            memcpy(response_buffer, serial_data.buffer+serial_data.frame_start+frame_size, response_frame_header.size_0);
                        }

                        switch(response_frame_header.command) {
                            case GET_MAC:
                                this->mac_ = str_snprintf("%02x:%02x:%02x:%02x:%02x:%02x", 17, response_buffer[0], 
                                    response_buffer[1], response_buffer[2], response_buffer[3], response_buffer[4], 
                                    response_buffer[5]).c_str();
                                break;

                            case READ_FIRMWARE:
                                this->version_ = str_snprintf("V%u.%02X.%02X%02X%02X%02X", 17, response_buffer[3], 
                                    response_buffer[2], response_buffer[7], response_buffer[6], response_buffer[5], 
                                    response_buffer[4]).c_str();
                                break;

#ifdef USE_NUMBER
                            case GET_REGIONS:
                                std::string value = this->regions_type_select_->at(sensor_regions.type).value();
                                ESP_LOGD(TAG, "ACK ,  %d", sensor_regions.type);
                                this->regions_type_select_->publish_state(value);

                                for(int i=0; i<3; i++) {
                                    number::Number *x0 = this->region_numbers_[i][0];
                                    number::Number *y0 = this->region_numbers_[i][1];
                                    number::Number *x1 = this->region_numbers_[i][2];
                                    number::Number *y1 = this->region_numbers_[i][3];

                                    if(x0 != nullptr) x0->publish_state(sensor_regions.coordinates[i].X0/10);
                                    if(y0 != nullptr) y0->publish_state(sensor_regions.coordinates[i].Y0/10);
                                    if(x1 != nullptr) x1->publish_state(sensor_regions.coordinates[i].X1/10);
                                    if(y1 != nullptr) y1->publish_state(sensor_regions.coordinates[i].Y1/10);
                                }
                                break;
#endif
                        }
                        serial_data.size = 0;
                        serial_data.frame_start = 0;
                    } else {
                        ESP_LOGD(TAG, "ACK Error, command %02x", response_frame_header.command);
                    }
                }
                break;
            }
        }

    }
}

void LD2450::dump_config(){
    ESP_LOGCONFIG(TAG, "LD2450:");
    ESP_LOGCONFIG(TAG, "  MAC Address : %s", const_cast<char *>(this->mac_.c_str()));
    ESP_LOGCONFIG(TAG, "  Firmware Version : %s", const_cast<char *>(this->version_.c_str()));
}

void LD2450::set_single_target(bool enable) {
    this->set_config_mode_(true);
    if(enable) this->set_single_target_();
    else this->set_multi_target_();
    this->set_config_mode_(false);
}

void LD2450::restore_factory() {
    this->set_config_mode_(true);
    this->send_command_(RESTORE_FACTORY, nullptr, 0);
    this->set_timeout(1000, [this]() { this->read_all_info(); });
}

void LD2450::set_bluetooth(bool enable) {
    this->set_config_mode_(true);
    uint8_t cmd_value[2] = { static_cast<uint8_t>(enable ? 0x01 : 0x00), 0x00};
    this->send_command_(SET_BLUETOOTH, cmd_value, 2);
    this->reboot_();
    this->set_timeout(1000, [this]() { this->read_all_info(); });
}

void LD2450::reboot_and_read() {
    this->set_config_mode_(true);
    this->reboot_();
    this->set_timeout(1000, [this]() { this->read_all_info(); });
}

void LD2450::set_baud_rate(uint8_t state) {
    this->set_config_mode_(true);
    uint8_t cmd_value[2] = {state, 0x00};
    this->send_command_(SET_BAUDRATE, cmd_value, 2);
    this->set_timeout(1000, [this]() { this->reboot_(); });
}

void LD2450::set_regions_type(uint8_t state) {
    this->set_config_mode_(true);
    uint8_t sensor_regions_size = sizeof(sensor_regions);
    sensor_regions.type = state;
    this->send_command_(SET_REGIONS, (uint8_t *)&sensor_regions, sensor_regions_size);
    this->set_config_mode_(false);
}

#ifdef USE_NUMBER
void LD2450::set_region_number(int region, int coord, number::Number *n) {
    this->region_numbers_[region][coord] = n;
}
#endif

#ifdef USE_NUMBER
void LD2450::set_region(uint8_t region) {
    number::Number *x0 = this->region_numbers_[region][0];
    number::Number *y0 = this->region_numbers_[region][1];
    number::Number *x1 = this->region_numbers_[region][2];
    number::Number *y1 = this->region_numbers_[region][3];

    if (x0->has_state()) sensor_regions.coordinates[region].X0 = x0->state*10;
    if (y0->has_state()) sensor_regions.coordinates[region].Y0 = y0->state*10;
    if (x1->has_state()) sensor_regions.coordinates[region].X1 = x1->state*10;
    if (y1->has_state()) sensor_regions.coordinates[region].Y1 = y1->state*10;
}
#endif

// Private Methods //////////////////////////////////////////////////////////////

void LD2450::report_position(void) {
#ifdef USE_SENSOR
    for(int i=0; i<3; i++) {
        if(person[i].x != 0 && person[i].y != 0) {
            person_before[i].x = person[i].x;
            person_before[i].y = person[i].y;
        }

        person[i] = rotate_coordinates(
            transform(received_data.person[i].cx)/10,
            -transform(received_data.person[i].cy)/10,
            rotate_angle
        );
    }

    PERSON_PUBLISH(position_x, 0, person[0].x);
    PERSON_PUBLISH(position_y, 0, person[0].y);
    PERSON_PUBLISH(speed, 0, transform(received_data.person[0].speed));
    PERSON_PUBLISH(resolution, 0, transform(received_data.person[0].resolution));

    PERSON_PUBLISH(position_x, 1, person[1].x);
    PERSON_PUBLISH(position_y, 1, person[1].y);
    PERSON_PUBLISH(speed, 1, transform(received_data.person[1].speed));
    PERSON_PUBLISH(resolution, 1, transform(received_data.person[1].resolution));

    PERSON_PUBLISH(position_x, 2, person[2].x);
    PERSON_PUBLISH(position_y, 2, person[2].y);
    PERSON_PUBLISH(speed, 2, transform(received_data.person[2].speed));
    PERSON_PUBLISH(resolution, 2, transform(received_data.person[2].resolution));
    

    if (this->target_count_sensor_ != nullptr) {
        uint8_t target_count = 0;
        for(int i=0; i<3; i++) {
            if(received_data.person[i].resolution) target_count++;
        }

        this->target_count_sensor_->publish_state(target_count);
    }
#endif

#ifdef USE_BINARY_SENSOR
    bool moving_target = (
        received_data.person[0].speed || 
        received_data.person[1].speed || 
        received_data.person[2].speed
    );

    bool still_target = (
        (received_data.person[0].resolution && !received_data.person[0].speed) ||
        (received_data.person[1].resolution && !received_data.person[1].speed) ||
        (received_data.person[2].resolution && !received_data.person[2].speed)
    );

    int32_t current_millis = millis();

    for (auto *presence_region : presence_regions) {
        presence_region->check_target(person);
    }

    for(int i=0; i<3; i++) {
        bool exiting=false;
        for (auto *entry_point : entry_points) {
            if(entry_point->check_point(person_before[i])) {
                exiting = true;
                break;
            }
        }

        if(received_data.person[i].resolution) {
            if(exiting) presence_millis[i] = 0;
            else presence_millis[i] = current_millis + presence_timeout*1000;
        }
    }

    bool target = (
        (received_data.person[0].resolution || presence_millis[0] > current_millis) ||
        (received_data.person[1].resolution || presence_millis[1] > current_millis) ||
        (received_data.person[2].resolution || presence_millis[2] > current_millis)
    );

    if (this->target_binary_sensor_ != nullptr) {
        this->target_binary_sensor_->publish_state(target);
    }
    if (this->moving_target_binary_sensor_ != nullptr) {
        this->moving_target_binary_sensor_->publish_state(moving_target);
    }
    if (this->still_target_binary_sensor_ != nullptr) {
        this->still_target_binary_sensor_->publish_state(still_target);
    }
#endif
}

int16_t LD2450::transform(uint16_t data) {
    return (data>>15) == 1 ? -1 * (data&0x7FFF) : data&0x7FFF;
}

#ifdef USE_NUMBER
void LD2450::set_rotate_number() {
    if (this->rotate_number_ != nullptr && this->rotate_number_->has_state()) {
        rotate_angle = this->rotate_number_->state;
    }
}
#endif

void LD2450::add_entry_point(EntryPoint *entry_point) { entry_points.emplace_back(entry_point); }

void LD2450::add_presence_region(PresenceRegion *presence_region) { presence_regions.emplace_back(presence_region); }

#ifdef USE_NUMBER
void LD2450::set_presence_timeout_number() {
    if (this->presence_timeout_number_ != nullptr && this->presence_timeout_number_->has_state()) {
        presence_timeout = this->presence_timeout_number_->state;
    }
}
#endif

coordinates LD2450::rotate_coordinates(double x, double y, double angle) {
    double angle_rad = angle * (M_PI / 180.0);
    
    double new_x = x * cos(angle_rad) - y * sin(angle_rad);
    double new_y = x * sin(angle_rad) + y * cos(angle_rad);
#ifdef INVERT_X
    new_x = -new_x;
#endif

#ifdef INVERT_Y
    new_y = -new_y;
#endif
    coordinates result = {new_x, new_y};
    return result;
}

void LD2450::send_command_(uint8_t command, const uint8_t *command_value, int command_value_len) {
    this->write_array(FRAME_HEADER, 4);

    int len = 2;
    if (command_value != nullptr) len += command_value_len;

    this->write_byte(lowbyte(len));
    this->write_byte(highbyte(len));

    this->write_byte(lowbyte(command));
    this->write_byte(highbyte(command));

    if (command_value != nullptr) {
        for (int i = 0; i < command_value_len; i++) {
            this->write_byte(command_value[i]);
        }
    }

    this->write_array(FRAME_END, 4);
}

void LD2450::read_all_info() {
    this->set_config_mode_(true);
    this->get_version_();
    this->get_mac_();
#ifdef USE_SELECT
    this->get_regions_();
#endif
    this->set_config_mode_(false);
}

void LD2450::set_config_mode_(bool enable) {
    uint8_t cmd = enable ? ENABLE_CONFIG : DISABLE_CONFIG;
    uint8_t cmd_value[2] = {0x01, 0x00};
    this->send_command_(cmd, enable ? cmd_value : nullptr, 2);
}

void LD2450::get_version_() { this->send_command_(READ_FIRMWARE, nullptr, 0); }

void LD2450::get_mac_() {
    uint8_t cmd_value[2] = {0x01, 0x00};
    this->send_command_(GET_MAC, cmd_value, 2);
}

void LD2450::get_regions_() { this->send_command_(GET_REGIONS, nullptr, 0); }

void LD2450::set_single_target_() { this->send_command_(SINGLE_TARGET, nullptr, 0); }

void LD2450::set_multi_target_() { this->send_command_(MULTI_TARGET, nullptr, 0); }

void LD2450::reboot_() { this->send_command_(REBOOT, nullptr, 0); }
}  // namespace ld2450
}  // namespace esphome

