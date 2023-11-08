#pragma once

typedef struct{
    uint16_t cx;
    uint16_t cy;
    uint16_t speed;
    uint16_t resolution;
}person_struct;

typedef struct{
    uint8_t data_header[4];
    person_struct person[3];
    uint8_t data_end[2];
}data_packet_struct;

typedef struct{
    uint8_t frame_header[4];
    uint8_t size_0;
    uint8_t size_1;
    uint8_t command;
    uint8_t command_ack;
    uint8_t reserved_0;
    uint8_t reserved_1;
} frame_start_struct;

typedef struct {
    int16_t X0;
    int16_t Y0;
    int16_t X1;
    int16_t Y1;
} region;

typedef struct {
    uint8_t type;
    uint8_t reserved;
    region coordinates[3];
} regions;

typedef struct{
    uint8_t buffer[256];
    uint8_t size;
    uint8_t frame_start;
}serial_buffer_struct;

typedef struct {
    double x;
    double y;
} coordinates;
