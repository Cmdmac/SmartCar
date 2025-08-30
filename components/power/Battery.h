#pragma once
#include "esp_adc/adc_oneshot.h"
#include "esp_adc/adc_cali.h"
#include "esp_adc/adc_cali_scheme.h"


#define VBAT_ADC_CHANNEL         ADC_CHANNEL_9  // S3: IO10

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

    int smoothVoltageArray[5] = {0};
    int smoothIndex = 0;
    int last_ewma_value;
    float ewma_alpha = 0.1; // EWMA平滑系数，取值范围为0到1

    int smoothVoltageEWMA(float voltage) {
        // EWMA公式：S_t = α·x_t + (1-α)·S_{t-1}
        last_ewma_value = ewma_alpha * voltage + (1 - ewma_alpha) * last_ewma_value;
        return last_ewma_value;
    }

    int smoothVoltageAverage(int voltage) {
        smoothVoltageArray[smoothIndex++] = voltage;
        smoothIndex = smoothIndex % 5;
        if (smoothVoltageArray[0] == 0 || smoothVoltageArray[1] == 0 || smoothVoltageArray[2] == 0 || smoothVoltageArray[3] == 0 || smoothVoltageArray[4] == 0) {
            return voltage;
        }
        return (smoothVoltageArray[0] + smoothVoltageArray[1] + smoothVoltageArray[2]  + smoothVoltageArray[3] + smoothVoltageArray[4]) / 5;
    }

public:
    Battery(gpio_num_t monitorPin) : monitorPin(monitorPin) {
        ewma_alpha = std::clamp(0.2f, 0.01f, 0.99f);
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
