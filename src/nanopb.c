#include "nanopb.h"


uint8_t decode_message_req(const uint8_t *buffer, size_t message_length, Dev_Msg *message){

    /* Function to decoding message from ProtoBuf*/
    pb_istream_t stream = pb_istream_from_buffer(buffer, message_length);

    if(!pb_decode(&stream, Dev_Msg_fields, message))
        return 0U;


    return 1U;
    
}

/*
Processing request and return message
status: 0 for fail and 1 for sucess
*/
uint8_t process_request(data_t *buffer){
    
    Dev_Msg msg = Dev_Msg_init_zero;
    uint8_t status = 0U;

    if(!decode_message_req(buffer->data, buffer->data_len, &msg))
        return 0U;

    /*Checking for correct uController*/
    if(msg.uController != RP2040_ZERO){
        status = encode_request(buffer, INVAILD_CMD, UNDEFINED_RESPOND);
        return status;
    }
        
    


    switch (msg.cmd)
    {
    case DEV_ACK:
        status = encode_request(buffer, DEV_ACK, MOTOR_DRIVER);
        break;
    
    default:
        status = encode_request(buffer, INVAILD_CMD, UNDEFINED_RESPOND);
        break;
    }


    return status;
}

uint8_t encode_request(data_t *buffer, uint8_t cmd, uint8_t req){
    
    Dev_Msg msg = Dev_Msg_init_zero;
    pb_ostream_t stream = pb_ostream_from_buffer(buffer->data, BUFFER_SIZE);
    

    msg.uController = RP2040_ZERO;
    msg.cmd         = cmd;
    msg.req         = req;

    if(!pb_encode(&stream, Dev_Msg_fields, &msg))
        return 0U;
    
    buffer->data_len = stream.bytes_written;

    return 1U;
}