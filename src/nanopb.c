#include "nanopb.h"



void nanopb_error(data_t *response){
    response->data[0] = 3U;
    response->data_len = 1U;

}

void write_stdio(data_t *buff){
    uint32_t idx;
    stdio_put_string(buff->data, buff->data_len, false, false);
    stdio_flush();

    for(idx = 0U; idx < buff->data_len; idx++){
        buff->data[idx] = 0U;
    }
    buff->data_len = 0U;
    
}

/**
 * Processing Special 1 bytes request
 */
uint8_t special_request(data_t *request, data_t *response){

    Assignment msg = Assignment_init_zero;
    pb_ostream_t stream;
    uint8_t status = 0U;
    if(request->data[0] == 0U){
        msg.asn = DevKind_MotorDriver;
        status = encode_request_msg(response, &msg);
    } else if(request->data[0] == 0xFF;){
        //watchdog_enable(0, false);
        response->data[0] = request->data[0];
        response->data_len = 1U;
        status = 1U;
    }

    return status;
}

uint8_t processing_msg_request(data_t *buffer, Assignment *msg){
    uint8_t status;
    switch (msg.asn)
    {
    case DevKind_HostAck:
        msg->asn = DevKind_DevAck;
        status = encode_request_msg(buffer, msg);
        break;
    
    default:
        nanopb_error(buffer);
        status = 0U;
        break;
    }
    return status;
}

uint8_t encode_request_msg(data_t *buffer, Assignment *msg){
    pb_ostream_t stream;
    stream = pb_ostream_from_buffer(buffer->data, PROTO_PACKET_SIZE);
    bool status = 0U;
    status = pb_encode(&stream, Assignment_fields, msg);

    if(!status)
        return 0U;

    buffer->data_len = stream.bytes_written;

    return 1U;
}


uint8_t decode_request_msg(data_t *buff, Assignment *message){

    /* Function to decoding message from ProtoBuf*/
    bool status;
    pb_istream_t stream = pb_istream_from_buffer(buff->data, buff->data_len);
    status = pb_decode(&stream, Assignment_fields, message);


    if(!status)
        return 0U;

    return 1U;
    
}


uint8_t encode_request_motor(data_t *buffer, MotorDriver_JRK *msg){
    pb_ostream_t stream;
    stream = pb_ostream_from_buffer(buffer->data, PROTO_PACKET_SIZE);
    bool status = 0U;
    status = pb_encode(&stream, MotorDriver_JRK_fields, msg);

    if(!status)
        return 0U;

    buffer->data_len = stream.bytes_written;

    return 1U;
}

uint8_t decode_request_motor(data_t *buff, MotorDriver_JRK *message){

    /* Function to decoding message from ProtoBuf*/
    bool status;
    pb_istream_t stream = pb_istream_from_buffer(buff->data, buff->data_len);
    status = pb_decode(&stream, MotorDriver_JRK_fields, message);


    if(!status)
        return 0U;

    return 1U;
    
}

uint8_t processing_motor_request(data_t *buffer, MotorDriver_JRK *motor){
    uint8_t status;
    status = encode_request_motor(buffer, motor);
    return status;
}

/*
Processing request and return message
status: 0 for fail and 1 for sucess
*/
uint8_t process_request(data_t *request, data_t *response){
    
    Assignment msg = Assignment_init_zero;
    MotorDriver_JRK motor = MotorDriver_JRK_init_zero;
    uint8_t status = 0U;
    pb_ostream_t stream;

    if(request->data[0] == 0U && request->data_len == 1U){
        status = special_request(request, response);
        return status;
    }

    status = decode_request_msg(request, &msg);
    if(status == 1U){
        status = processing_msg_request(response, &msg);
        return status;
    }

    status = decode_request_motor(request, &motor);
    if(status == 1U){

    }

    if(status == 0U){
        nanopb_error(response);
    }



    return status;
}