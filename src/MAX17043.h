#pragma once

#include <I2CDevice.h>

// Device info
#define MAX17043_ADDRESS  0x36

// Register map
enum {
    MAX17043_RA_VCELL   = 0x02, // R
    MAX17043_RA_SOC     = 0x04, // R
    MAX17043_RA_MODE    = 0x06, // W
    MAX17043_RA_VERSION = 0x08, // R
    MAX17043_RA_CONFIG  = 0x0C, // R/W
    MAX17043_RA_COMMAND = 0xFE  // W
};

// MODE
#define MAX17043_MODE_QUICKSTART    0x4000

// CONFIG
#define MAX17043_CONFIG_ATHD        0
#define MAX17043_CONFIG_ALRT        5
#define MAX17043_CONFIG_SLEEP       7
#define MAX17043_CONFIG_RCOMP       8

// COMMAND
#define MAX17043_COMMAND_POR        0x5400

class MAX17043 : public I2CDevice {
public:
    static MAX17043& getInstance() {
        static MAX17043 instance;
        return instance;
    }

    // Initialization
    MAX17043() : I2CDevice(MAX17043_ADDRESS) {

    }

    void initialize() {
        quickStart();
    }

    bool testConnection() {
        // TODO: Check if this is the right chip
        return true;
    }

    // VCELL register
    float getVoltage() {
        uint16_t rawVoltage;
        readWord(MAX17043_RA_VCELL, &rawVoltage);

        // 12-bit register
        rawVoltage = (rawVoltage) >> 4;

        // Each bit is 1.25mv
        return rawVoltage / (1000.0 / 1.25);
    }

    // SOC register
    float getStateOfCharge() {
        uint16_t rawSoc;
        readWord(MAX17043_RA_SOC, &rawSoc);

        // High byte is percentage
        float percentage = (uint8_t) (rawSoc >> 8);  

        // Low byte is 1/256%
        percentage += (float) (((uint8_t) rawSoc)/256.0);

        return percentage;
    }

    // MODE register
    void quickStart() {
        writeWord(MAX17043_RA_MODE, MAX17043_MODE_QUICKSTART);
    }

    // VERSION register
    uint16_t getVersion() {
        uint16_t version;
        readWord(MAX17043_RA_VERSION, &version);

        return version;
    }

    // CONFIG register
    uint8_t getAlertThreshold() {
        uint8_t alertThreshold;
        // TODO: Get 5-bit value from MAX17043_RA_CONFIG, bit MAX17043_CONFIG_ATHD
        return alertThreshold;
    }

    void setAlertThreshold() {
        // TODO: Write 5-bit value to MAX17043_RA_CONFIG, bit MAX17043_CONFIG_ATHD
    }

    bool getAlert() {
        readWordBit(MAX17043_RA_CONFIG, MAX17043_CONFIG_ALRT, buffer);
        
        return buffer[0];
    }

    void clearAlert() {
        writeWordBit(MAX17043_RA_CONFIG, MAX17043_CONFIG_ALRT, 0);
    }

    void sleep() {
        writeWordBit(MAX17043_RA_CONFIG, MAX17043_CONFIG_SLEEP, 1);
    }

    void wake() {
        writeWordBit(MAX17043_RA_CONFIG, MAX17043_CONFIG_SLEEP, 0);
    }

    uint8_t getCompensation() {
        uint8_t compensation;
        // TODO: Get 8-bit value from MAX17043_RA_CONFIG, bit MAX17043_CONFIG_RCOMP
        return compensation;
    }

    void setCompensation(uint8_t compensation) {
        // TODO: Set 8-bit value to MAX17043_RA_CONFIG, bit MAX17043_CONFIG_RCOMP
    }

    // COMMAND register
    void reset() {
        writeWord(MAX17043_RA_COMMAND, MAX17043_COMMAND_POR);
    }
};
