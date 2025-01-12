#ifndef SERIAL_USB_H
#define SERIAL_USB_H

#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "generated/ws2812.pio.h"




void put_pixel(uint32_t pixel_grb);
void put_rgb(uint8_t green, uint8_t red, uint8_t blue);

#endif