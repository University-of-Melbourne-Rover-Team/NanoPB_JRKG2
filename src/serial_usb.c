#include "serial_usb.h"
#include "nanopb.h"
#include "stdio.h"

void put_pixel(uint32_t pixel_grb)
{
    pio_sm_put_blocking(pio0, 0, pixel_grb << 8u);
}
void put_rgb(uint8_t red, uint8_t green, uint8_t blue)
{
    uint32_t mask = (green << 16) | (red << 8) | (blue << 0);
    put_pixel(mask);
}

void usb_uart_task(){

    data_t tx_buffer;
    uint8_t cmd = DEV_ACK;
    uint8_t dev = MOTOR_DRIVER;
    Dev_Msg msg = Dev_Msg_init_zero;

    if(!encode_request(&tx_buffer, cmd, dev)){
        printf("Fail to Encode\n");
        return;
    }
        
    size_t written = fwrite(tx_buffer.data, tx_buffer.data_len, 1, stdout);
    fflush(stdout);
    sleep_ms(1000);

    printf("%c",(char) 0xff);
    decode_message_req(tx_buffer.data,tx_buffer.data_len,&msg);
    printf("%c%c",msg.cmd,msg.req);

    
    sleep_ms(1000);
}