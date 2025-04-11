#pragma once
#include "esp_adc/adc_oneshot.h"
#include "esp_adc/adc_cali.h"
#include "esp_adc/adc_cali_scheme.h"

#define VBAT_ADC_CHAN           ADC_CHANNEL_1
// #define VBAT_ADC_ATTEN          ADC_ATTEN_DB_12
// #define VBAT_MONITOR_GPIO       (GPIO_NUM_2)
#define VBAT_CHARGING_LEVEL     0
#define BATTERY_VOLTAGE_PERCENT 20

#define ADC_SMOOTH_WINDOW_SIZE  10

class Battery
{
private:
    int estimateCapacity(float voltage);
    int batteryVoltageSmoothing(int new_sample);
    bool adc_calibration_init(adc_unit_t unit, adc_channel_t channel, adc_atten_t atten, adc_cali_handle_t *out_handle);
    /*
    锂电池电压与剩余电量关系：
    电压 (V)	电量 (%)
    4.20	100%
    3.90	75%
    3.70	50%
    3.50	25%
    3.30	0%
    */
    float voltageToPercent(float voltage) {
        if (voltage >= 4.2) return 100;
        else if (voltage >= 3.9) return 75 + (voltage - 3.9) * 25 / 0.3; // 3.9~4.2V对应75%~100%
        else if (voltage >= 3.7) return 50 + (voltage - 3.7) * 25 / 0.2; // 3.7~3.9V对应50%~75%
        else if (voltage >= 3.5) return 25 + (voltage - 3.5) * 25 / 0.2; // 3.5~3.7V对应25%~50%
        else return 0;
    }
    gpio_num_t monitorPin;
    int adc_raw[2][10];
    int voltage[2][10];

    int last_vbat_charging_state = !VBAT_CHARGING_LEVEL;

    int percent;
    bool is_calibration;
    adc_oneshot_unit_handle_t adc1_handle;
    adc_oneshot_unit_init_cfg_t init_config1;
    adc_cali_handle_t adc1_cali_chan0_handle;
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
