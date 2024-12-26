#include <pb_encode.h>
#include <pb_decode.h>
#include "uart.pb.h"
#include "stdio.h"
#include "pico/stdlib.h"
#include "stdlib.h"
#include "nanopb.h"

#define BUFFER_SIZE 128


// Main function
int main(int argc, char **argv) {
    
    //Initialise I/O
    stdio_init_all();
    sleep_ms(1000);
    data_t buffer;
    uint8_t status;
    


    return 0;
}