#include "jrkg2.h"
#include "stdint.h"
#include "stdbool.h"


#define MAX_BUFFER_LENGTH 13
#define I2C_HEADER_SIZE 2
#define ONE_BYTE    1
#define TWO_BYTES   2
#define THREE_BYTES 3

jrkg2_status_t setPin(i2c_inst_t *I2C_PORT,int sda_pin, int sdl_pin){
    jrkg2_status_t status = JRK_G2_UNDEFINED;

      if(sda_pin >= 0 && sda_pin <= 12 && (sda_pin == (sdl_pin-1))){
        if(sda_pin%4 == 0){
            I2C_PORT = i2c0;
        } else{
            I2C_PORT = i2c1;
        }
        
        i2c_init(I2C_PORT, 400000);
        gpio_set_function(sda_pin,GPIO_FUNC_I2C);
        gpio_set_function(sdl_pin,GPIO_FUNC_I2C);
        gpio_pull_up(sda_pin);
        gpio_pull_up(sdl_pin);
        status = JRK_G2_SET_PIN_SUCESS;
    } else {
        status = JRK_G2_SET_PIN_FAIL;
    }
    return status;
}


jrkg2_status_t cmdQuick(i2c_inst_t *I2C_PORT,uint8_t addr, uint8_t cmd){
    /*Only for sending specific command like setting mode etc*/
    jrkg2_status_t status = JRK_G2_WRITE_SUCCESS;
    if (i2c_write_blocking(I2C_PORT, addr, &cmd,ONE_BYTE, false) < 0)
        status = JRK_G2_WRITE_FAIL;
    
    return status;
}



jrkg2_status_t cmdWrite7(i2c_inst_t *I2C_PORT,uint8_t address,uint8_t cmd, uint8_t val){
    
    /* Writing 7 bits via I2C */
    jrkg2_status_t status = JRK_G2_WRITE_SUCCESS;
    uint8_t bufferByte[2];
    bufferByte[0] = cmd;
    bufferByte[1] = (val & 0x7F);
    if(i2c_write_blocking(I2C_PORT,address,bufferByte,TWO_BYTES,false) < 0)
        status = JRK_G2_WRITE_FAIL;
    
    return status;
}


jrkg2_status_t cmdWrite14(i2c_inst_t *I2C_PORT, uint8_t address,uint8_t cmd, uint16_t val){
    /* Writing 14 bits via I2C */
    jrkg2_status_t status = JRK_G2_WRITE_SUCCESS;
    uint8_t bufferByte[3];
    bufferByte[0] = cmd;
    bufferByte[1] = (val & 0xFF);
    bufferByte[2] = (val >> 8 & 0xFF);
     if (i2c_write_blocking(I2C_PORT,address,bufferByte,THREE_BYTES,false) < 0)
            status = JRK_G2_WRITE_FAIL;

    
    return status;
}



uint8_t cmdRead8(i2c_inst_t *I2C_PORT,uint8_t address,uint8_t cmd){

    /* Reading 8 bits via I2C */
    uint8_t bufferByte;
    i2c_write_blocking(I2C_PORT,address,&cmd,ONE_BYTE,true);
    i2c_read_blocking(I2C_PORT,address, &bufferByte,ONE_BYTE,false);

    return bufferByte;

}


uint16_t cmdRead16(i2c_inst_t *I2C_PORT, uint8_t address,uint8_t cmd){
    /* Reading 16 bits via I2C */
    uint8_t bufferByte[2]; 
    uint16_t dataByte;

    i2c_write_blocking(I2C_PORT,address,&cmd,ONE_BYTE,true);
    i2c_read_blocking(I2C_PORT,address,bufferByte,TWO_BYTES,true);

    dataByte = (bufferByte[1] << 8) | bufferByte[0];
    return dataByte;
}

jrkg2_status_t setTargert(i2c_inst_t *I2C_PORT, uint8_t address, uint16_t target){


    if(target > 4095)
        target = 4095;
    
    jrkg2_cmd_t cmd = SetTargetHighRes;

    return cmdWrite7(I2C_PORT, address,(uint8_t) SetTargetHighRes | (target & 0x1F), target >> 5);
}

jrkg2_status_t motorOff(i2c_inst_t *I2C_PORT, uint8_t address){

    jrkg2_cmd_t cmd = StopMotor;
    return cmdQuick(I2C_PORT, address,(uint8_t) cmd);
}


jrkg2_status_t setForceDutyCycle(i2c_inst_t *I2C_PORT ,uint8_t address, int16_t duty_cycle){

    if(duty_cycle < -600)
        duty_cycle = -600;

    if(duty_cycle > 600)
        duty_cycle = 600;;
    
    return cmdWrite14(I2C_PORT,address ,ForceDutyCycle, duty_cycle);
}

jrkg2_status_t readVariable(i2c_inst_t *I2C_PORT, uint8_t address, uint8_t offset, jrkg2_data_t *buffer) {
    

    /* The data need to free afterward*/
    if (buffer->length > MAX_BUFFER_LENGTH) {
        buffer->length = MAX_BUFFER_LENGTH;
    }

    uint8_t data[I2C_HEADER_SIZE] = {GetVariables, offset};

    // Allocate memory for buffer->data
    buffer->data = (uint8_t *) malloc(buffer->length);
    if (buffer->data == NULL) {
        return JRK_G2_READ_FAIL; 
    }

    
    if (i2c_write_blocking(I2C_PORT, address, data, I2C_HEADER_SIZE, true) < 0 ||
        i2c_read_blocking(I2C_PORT, address, buffer->data, buffer->length, false) < 0) {
        free(buffer->data); 
        return JRK_G2_READ_FAIL; 
    }

    return JRK_G2_READ_SUCCESS;
}

jrkg2_status_t writeRAM(i2c_inst_t *I2C_PORT, uint8_t address, uint8_t offset, jrkg2_data_t *buffer) {
    // Validate buffer length
    if (buffer->length > MAX_BUFFER_LENGTH) {
        buffer->length = MAX_BUFFER_LENGTH;
    }

    // Allocate memory for the data buffer
    uint8_t *data = (uint8_t *) malloc(buffer->length + I2C_HEADER_SIZE);
    if (data == NULL) {
        return JRK_G2_WRITE_FAIL; 
    }

    // Populate the data buffer
    data[0] = SetRAMSettings;
    data[1] = offset;
    memcpy(data + I2C_HEADER_SIZE, buffer->data, buffer->length);

    // Perform I2C write operation
    if (i2c_write_blocking(I2C_PORT, address, data, buffer->length + I2C_HEADER_SIZE, false) < 0) {
        free(data); 
        return JRK_G2_WRITE_FAIL; 
    }

    // Free the allocated memory
    free(data);
    return JRK_G2_WRITE_SUCCESS;
}