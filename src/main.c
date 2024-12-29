#include "stdio.h"

#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"

#include "stdlib.h"
#include "nanopb.h"

#include "cdc_usb.h"


#define BUFFER_SIZE 128

//Board Macro
#define RP2040_ZERO_BOARD 1
#define RASP_PIPICO_BOARD 0

// Main function
int main(int argc, char **argv) {
    
    //Initialise I/O
    stdio_init_all();
    sleep_ms(1000);
    data_t buffer;
    uint8_t status;


    //Start LED
    if(RP2040_ZERO_BOARD){
        PIO pio = pio0;
        int sm = 0;
        uint offset = pio_add_program(pio, &ws2812_program);
        
        ws2812_program_init(pio, sm, offset, 16, 800000, true);

        //Setting LED to Red during startup
        put_rgb(252, 0 , 0);
    }


    put_rgb(0, 252, 0);

    return 0;
}