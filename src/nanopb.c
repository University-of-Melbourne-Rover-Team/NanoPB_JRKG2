#include "nanopb.h"


uint8_t decode_message_req(const uint8_t *buffer, size_t message_length, Assignment *message){

    /* Function to decoding message from ProtoBuf*/
    pb_istream_t stream = pb_istream_from_buffer(buffer, message_length);

    if(!pb_encode(&stream, Assignment_fields, &message))
        return 0U;


    return 1U;
    
}


void nanopb_error(data_t *response){
    response->data[0] = 3U;
    response->data_len = 1U;

}

void write_stdio(data_t *buff){
    uint32_t idx;
    for(idx = 0; idx < buff->data_len; idx++){
        stdio_putchar_raw(buff->data[idx]);
    }
}


/*
Processing request and return message
status: 0 for fail and 1 for sucess
*/
uint8_t process_request(data_t *request, data_t *response){
    
    Assignment msg = Assignment_init_zero;
    uint8_t status = 0U;

    if(!decode_message_req(request->data, request->data_len, &msg)){
        status = 0U;
        nanopb_error(response);
    }
    
    if(status != 0U){
        switch (msg.asn)
        {
        case DevKind_DevAck:
            msg.asn = DevKind_Battery;
            status = encode_request(response, &msg);
            break;
    
        default:
            nanopb_error(response);
            status = 0U;
            break;
        }
    }


    return status;
}

uint8_t encode_request(data_t *buffer, Assignment *msg){
    
    pb_ostream_t stream = pb_ostream_from_buffer(buffer->data, BUFFER_SIZE);
    
    if(!pb_encode(&stream, msg, &msg))
        return 0U;
    
    buffer->data_len = stream.bytes_written;

    return 1U;
}