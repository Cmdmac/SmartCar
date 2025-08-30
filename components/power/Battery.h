#pragma once
#include "esp_adc/adc_oneshot.h"
#include "esp_adc/adc_cali.h"
#include "esp_adc/adc_cali_scheme.h"


#define VBAT_ADC_CHANNEL         ADC_CHANNEL_9  // S3: IO10
#define MCU_VCC_CTL              GPIO_NUM_4     // set 1 to power on MCU
#define PERP_VCC_CTL             GPIO_NUM_6     // set 1 to power on peripherals

#define ADC_ATTEN                ADC_ATTEN_DB_12
#define ADC_WIDTH                ADC_BITWIDTH_DEFAULT
#define FULL_BATTERY_VOLTAGE     4100
#define EMPTY_BATTERY_VOLTAGE    3200

class Battery
{
private:
    void initializeADC();
        
    gpio_num_t monitorPin;
    adc_oneshot_unit_handle_t adc1_handle;
    adc_cali_handle_t adc1_cali_handle;
    bool do_calibration = false;

    int percent;

    int smoothVoltageArray[3] = {0};
    int smoothIndex = 0;

    int smoothVoltage(int voltage) {
        smoothVoltageArray[smoothIndex++] = voltage;
        smoothIndex = smoothIndex % 3;
        if (smoothVoltageArray[0] == 0 || smoothVoltageArray[1] == 0 || smoothVoltageArray[2] == 0) {
            return voltage;
        }
        return (smoothVoltageArray[0] + smoothVoltageArray[1] + smoothVoltageArray[2]) / 3;
    }

public:
    Battery(gpio_num_t monitorPin) : monitorPin(monitorPin) {

    }
    // ~Battery();

    void setup();

    int getPercent() {
        return this->percent;
    }
    
    int detect();
};

// Battery::Battery() : percent(0)
// {
// }

// Battery::~Battery()
// {
// }
