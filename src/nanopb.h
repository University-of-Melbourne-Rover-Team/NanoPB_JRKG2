#ifndef NANOPB_PI_H
#define NANOPB_PI_H


#include <pb_encode.h>
#include <pb_decode.h>
#include "proto/uart.pb.h"
#include "stdio.h"
#include "pico/stdlib.h"


#define PROTO_PACKET_SIZE  64U
#define PROTO_PACKET_COUNT 4U
#define USB_UART_BUFFER    PROTO_PACKET_SIZE*PROTO_PACKET_COUNT

#define RP2040_ZERO 2040
#define RP_PI_PICO  2041

#define RX_BUFFER_OVERFLOW (1U << 31)
#define RX_BUFFER_HAS_DATA (1U << 30)

typedef struct 
{
    uint8_t data[USB_UART_BUFFER];
    uint32_t data_len;
    uint32_t event;
    uint8_t  packet_cnt;
}data_t;


void write_stdio(data_t *buff);
void nanopb_error(data_t *response);

uint8_t special_request(data_t *request, data_t *response);
uint8_t processing_msg_request(data_t *buffer, Assignment *msg);
uint8_t encode_request_msg(data_t *buffer, Assignment *msg);
uint8_t decode_request_msg(data_t *buff, Assignment *message);
uint8_t encode_request_motor(data_t *buffer, MotorDriver_JRK *msg);
uint8_t decode_request_motor(data_t *buff, MotorDriver_JRK *message);
uint8_t process_request(data_t *request, data_t *response);
#endif