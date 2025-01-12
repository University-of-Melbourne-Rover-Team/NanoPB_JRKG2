#ifndef NANOPB_PI_H
#define NANOPB_PI_H


#include <pb_encode.h>
#include <pb_decode.h>
#include "proto/uart.pb.h"
#include "stdio.h"
#include "pico/stdlib.h"



#define BUFFER_SIZE 128

#define RP2040_ZERO 2040
#define RP_PI_PICO  2041



#define INVAILD_CMD    0x10
#define DEV_ACK        0x11
#define FAILED_DECODED 0x12

/*Device Type*/
#define UNDEFINED_RESPOND 0x20
#define MOTOR_DRIVER      0x21
#define CAM_GIMBALL       0x22
#define SCIENCE           0x23
#define BATTERY           0x24
#define ARM_TELEM         0x25
#define DEBRIS_CLEAR      0x26
#define CHASIS_LIGHTS     0x27

#define UART_BUFFER 256
typedef struct 
{
    uint8_t data[UART_BUFFER];
    size_t data_len;
}data_t;



uint8_t decode_message_req(const uint8_t *buffer, size_t message_length, Dev_Msg *message);
uint8_t process_request(data_t *buffer);
uint8_t encode_request(data_t *buffer, uint8_t cmd, uint8_t req);


#endif