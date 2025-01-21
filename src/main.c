#include "stdio.h"

#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"

#include "stdlib.h"
#include "nanopb.h"
#include "serial_usb.h"

#define START_BYTES { 0x52, 0x50, 0x32, 0x30, 0x34, 0x30 } //RP2040

//Board Macro
#define RP2040_ZERO_BOARD 1
#define RASP_PIPICO_BOARD 0

void start_LED(){
    
    if(RP2040_ZERO_BOARD){
        PIO pio = pio0;
        int sm = 0;
        uint offset = pio_add_program(pio, &ws2812_program);
        
        ws2812_program_init(pio, sm, offset, 16, 800000, true);

        //Setting LED to Red during startup
        put_rgb(0, 0 , 0);
    }

}

void stdio_callback(void *param){
    data_t *rxbuffer = (data_t *)param;
    int byte;
    rxbuffer->event |= RX_BUFFER_HAS_DATA;
    while (true)
    {
       byte = stdio_getchar_timeout_us(10);
       if(byte == PICO_ERROR_TIMEOUT)
            break;
        
        
        if(rxbuffer->data_len >= UART_BUFFER-1){
            rxbuffer->event |= RX_BUFFER_OVERFLOW;
            break;
        }
        
        rxbuffer->data[rxbuffer->data_len++] = (uint8_t) byte;
    }
    
}


// Main function
int main(int argc, char **argv) {
    
    //Initialise I/O
    stdio_init_all();

    data_t rxBuffer = {.data = {0U}, .data_len=0U, .event=0U};
    data_t txBuffer = {.data = {0U}, .data_len=0U, .event=0U};

    stdio_set_chars_available_callback(stdio_callback,&rxBuffer);
    sleep_ms(1000);

    //Todo Implement handshake

    while (true)
    {   
        if( rxBuffer.event & RX_BUFFER_HAS_DATA){
            process_request(&rxBuffer, &txBuffer);
            rxBuffer.data_len = 0U;
            rxBuffer.event = 0U;
            write_stdio(&txBuffer);
        } else {
            sleep_ms(1);
        }
        
    }
    

    return 0;
}


