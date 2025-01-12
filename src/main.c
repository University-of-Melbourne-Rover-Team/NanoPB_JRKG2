#include "stdio.h"

#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"

#include "stdlib.h"
#include "nanopb.h"
#include "serial_usb.h"



#define BUFFER_SIZE 128

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


// Main function
int main(int argc, char **argv) {
    
    //Initialise I/O
    stdio_init_all();
    stdio_usb_init();
    sleep_ms(1000);
    data_t buffer;
    uint8_t status;

    int start = 10U;


    //Start LED
    //start_LED();

    while (start != 1U)
    {   
        printf("%c",(char) 0xff);
        scanf("%i",&start);
    }
    

    while (true)
    {
        usb_uart_task();
    }
    

    return 0;
}


