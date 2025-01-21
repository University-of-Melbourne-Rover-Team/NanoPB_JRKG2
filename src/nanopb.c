#include "nanopb.h"


uint8_t decode_message_req(const uint8_t *buffer, size_t message_length, Assignment *message){

    /* Function to decoding message from ProtoBuf*/
    pb_istream_t stream = pb_istream_from_buffer(buffer, message_length);

    if(!pb_decode(&stream, Assignment_fields, message))
        return 0U;


    return 1U;
    
}


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
    
}


/*
Processing request and return message
status: 0 for fail and 1 for sucess
*/
uint8_t process_request(data_t *request, data_t *response){
    
    Assignment msg = Assignment_init_zero;
    uint8_t status = 0U;
    pb_ostream_t stream;
    if(request->data[0] == 0U && request->data_len == 1U){
        msg.asn = DevKind_Battery;
        stream = pb_ostream_from_buffer(response->data, BUFFER_SIZE);
        pb_encode(&stream, Assignment_fields, &msg);
        response->data_len = stream.bytes_written;
        status = 1U;
        return status;
    }


    pb_istream_t out_stream = pb_istream_from_buffer(request->data, request->data_len);
    status = pb_decode(&out_stream, Assignment_fields, &msg);

    // if(!decode_message_req(request->data, request->data_len, &msg)){
    //     status = 0U;
    //     nanopb_error(response);
    // }
    
    if(!status){
        switch (msg.asn)
        {
        case DevKind_HostAck:
            msg.asn = DevKind_DevAck;
            stream = pb_ostream_from_buffer(response->data, BUFFER_SIZE);
            pb_encode(&stream, Assignment_fields, &msg);
            response->data_len = stream.bytes_written;
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
    
    if(!pb_encode(&stream, Assignment_fields, msg))
        return 0U;
    
    buffer->data_len = stream.bytes_written;

    return 1U;
}