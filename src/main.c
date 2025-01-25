#include "stdio.h"

#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"

#include "stdlib.h"
#include "nanopb.h"
#include "serial_usb.h"
#include "hardware/sync.h"




data_t rxBuffer = {.data = {0U}, .data_len=0U, .event=0U};
data_t txBuffer = {.data = {0U}, .data_len=0U, .event=0U};

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
    int byte;
    rxBuffer.event |= RX_BUFFER_HAS_DATA;
    while (true)
    {
       byte = stdio_getchar_timeout_us(10);
       if(byte == PICO_ERROR_TIMEOUT)
            break;
        
        
        if(rxBuffer.data_len >= USB_UART_BUFFER-1U){
            rxBuffer.event |= RX_BUFFER_OVERFLOW;
            break;
        }
        
        rxBuffer.data[rxBuffer.data_len++] = (uint8_t) byte;
    }
    
}


// Main function
int main(int argc, char **argv) {
    
    //Initialise I/O
    stdio_init_all();
    uint32_t irq_status;
    stdio_set_chars_available_callback(stdio_callback, NULL);

    while (true)
    {   
        if( rxBuffer.event & RX_BUFFER_HAS_DATA){
            irq_status = save_and_disable_interrupts();
            process_request(&rxBuffer, &txBuffer);
            rxBuffer.data_len = 0U;
            rxBuffer.event = 0U;
            write_stdio(&txBuffer);
            restore_interrupts_from_disabled(irq_status);
        } else {
            sleep_ms(1);
        }
        
    }
    

    return 0;
}


