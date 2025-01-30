#ifndef JRKG2_H
#define JRKG2_H

#include "stdint.h"
#include "stdlib.h"
#include "pico/stdlib.h"
#include "hardware/i2c.h"


typedef struct
{
    uint8_t *data;
    size_t length; 
} jrkg2_data_t;



typedef enum{
    JRK_G2_UNDEFINED,
    JRK_G2_SET_PIN_SUCESS,
    JRK_G2_SET_PIN_FAIL,
    JRK_G2_WRITE_SUCCESS,
    JRK_G2_WRITE_FAIL,
    JRK_G2_READ_SUCCESS,
    JRK_G2_READ_FAIL,
    
} jrkg2_status_t;


typedef enum{
    Input                           = 0x00, //unsigned 16-bit
    Target                          = 0x02, //unsigned 16-bit
    Feedback                        = 0x04, //unsigned 16-bit
    ScaledFeedback                  = 0x06, //unsigned 16-bit
    Intergal                        = 0x08, //signed 16-bit
    DutyCycleTarget                 = 0x0A, //signed 16-bit
    DutyCyle                        = 0x0C, //signed 16-bit
    CurrentLowRes                   = 0x0E, //unsigned 8-bit
    PIDPeriodExceeded               = 0x0F, //boolean at bit 0
    PIDPeriodCount                  = 0x10, //unsigned 16-bit
    ErrorFlagsHalt                  = 0x12, //unsigned 16-bit
    ErrorFlagsOccured               = 0x14, //unsigned 16-bit
    ForceMode                       = 0x16, //0 to 2 with bit 0 and 1
    VINVoltage                      = 0x17, //unsigned 16-bit unit in mV
    Current                         = 0x19, //unsigned 16-bit uint in mA
    LastReset                       = 0x1F, //unsigned 8-bit page 142 in manual for more info
    UpTime                          = 0x20, //unsigned 32-bit 0 to 4,294,967,295 in ms
    RCPulseWidth                    = 0x24, //unsigned 16-bit 2400 (200 µs) to 32400 (2700 µs)
    FBTReading                      = 0x26, //unsigned 16-bit
    AnalogReadingSDAAN              = 0x28, //unsigned 16-bit
    AnalogReadingFBA                = 0x2A, //unsigned 16-bit
    DigitalReadings                 = 0x2C, //unsigned 8-bit
    RawCurrent                      = 0x2D, //unsigned 16-bit
    HardCurrentLimit                = 0x2F, //unsigned 16-bit
    LastDutyCyle                    = 0x31, //signed 16-bit
    CurrentChoppingConsectiveCount  = 0x33, //unsigned 8-bit
    CurrentChoppingOccurenceCount   = 0x34  //unsigned 8-bit
} jrkg2_var_offset_t;


typedef enum{
    OptionByte1                    = 0x01,
    OptionByte2                    = 0x02,
    InputMode                      = 0x03,
    InputErrorMin                  = 0x04,
    InputErrorMax                  = 0x06,
    InputMin                       = 0x08,
    InputMax                       = 0x0A,
    InputNeutural                  = 0x0C,
    InputNeturalMax                = 0x0E,
    TargetMin                      = 0x10,
    TargetNetural                  = 0x12,
    TargetMax                      = 0x14,
    InputScalingDegree             = 0x16,
    InputAnalogSmaplesExponent     = 0x17,
    FeedbackMode                   = 0x18,
    FeedbackErrMin                 = 0x19,
    FeedbackErrMax                 = 0x1B,
    FeedbackMin                    = 0x1D,
    FeedbackMax                    = 0x1F,
    SerialMode                     = 0x23,
    SerialBaudRate                 = 0x24,
    SerialTimeout                  = 0x26,
    SerialDeviceNumber             = 0x28,
    ErrorEnable                    = 0x2A,
    ErrorLatch                     = 0x2C,
    ErrorHardStop                  = 0x2E,
    VINCalibration                 = 0x30,
    PWMFrequency                   = 0x32,
    CurrentSampleExpoent           = 0x33,
    HardOverCurrentThreshold       = 0x34,
    CurrentOffsetCalibration       = 0x35,
    CurrentScaleCalibration        = 0x37,
    FBTMethod                      = 0x39,
    FBTTimingOptionByte            = 0x3A,
    FBTSamples                     = 0x3E,
    IntegralDividerExpoent         = 0x3F,
    PropMultiplier                 = 0x51,
    OptionByte3                    = 0x50,
    PropExponent                   = 0x53,
    IntegralMultiplier             = 0x54,
    IntegralExponent               = 0x56,
    DerivativeMultiplier           = 0x57,
    DerivativeExponent             = 0x59,
    PIDPeriod                      = 0x5A,
    IntegralLimit                  = 0x5C,
    MaxDutyCycleFeedbackOutOfRange = 0x5E,
    MaxAccelFwd                    = 0x60,
    MaxAccelRev                    = 0x62,
    MaxDecelFwd                    = 0x64,
    MaxDecelRev                    = 0x66,
    MaxDutyCycleFwd                = 0x68,
    MaxDutyCycleRev                = 0x6A,
    HardCurrentLimitFwd            = 0x6C,
    HardCurrentLimitRev            = 0x6E,
    BrakeDurationFwd               = 0x70,
    BrakeDurationRev               = 0x71,
    SoftCurrentLimitFwd            = 0x72,
    SoftCurrentLimitRev            = 0x74,
    SoftCurrentRegulationLvlFwd    = 0x40,
    SoftCurrentRegulationLvlRev    = 0x42,
} jrkg2_setting_offset_t;

typedef enum{
        SetTargetHighRes        = 0xC0,
        SetTargetLowResFwd      = 0xE1,
        SetTargetLowResRev      = 0xE0,
        StopMotor               = 0xFF,
        ForceDutyCycleTarget    = 0xF2,
        ForceDutyCycle          = 0xF4,
        SetRAMSettings          = 0xE6,
        GetRAMSettings          = 0xEA,
        GetEEPROMSettings       = 0xE3,
        GetVariables            = 0xE5,
        GetVariableOneByte      = 0xA1,
        GetVariableTwoByte      = 0x81
} jrkg2_cmd_t;

jrkg2_status_t setPin(i2c_inst_t *I2C_PORT,int sda_pin, int sdl_pin);
jrkg2_status_t cmdQuick(i2c_inst_t *I2C_PORT,uint8_t addr, uint8_t cmd);
jrkg2_status_t cmdWrite7(i2c_inst_t *I2C_PORT,uint8_t address,uint8_t cmd, uint8_t val);
jrkg2_status_t cmdWrite14(i2c_inst_t *I2C_PORT, uint8_t address,uint8_t cmd, uint16_t val);
jrkg2_status_t setTargert(i2c_inst_t *I2C_PORT, uint8_t address, uint16_t target);
jrkg2_status_t motorOff(i2c_inst_t *I2C_PORT, uint8_t address);
jrkg2_status_t setForceDutyCycle(i2c_inst_t *I2C_PORT ,uint8_t address, int16_t duty_cycle);
jrkg2_status_t writeRAM(i2c_inst_t *I2C_PORT, uint8_t address, uint8_t offset, jrkg2_data_t *buffer);
jrkg2_status_t readVariable(i2c_inst_t *I2C_PORT, uint8_t address, uint8_t offset, jrkg2_data_t *buffer);

uint8_t cmdRead8(i2c_inst_t *I2C_PORT,uint8_t address,uint8_t cmd);
uint16_t cmdRead16(i2c_inst_t *I2C_PORT, uint8_t address,uint8_t cmd);


#endif