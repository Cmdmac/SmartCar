#include <Arduino.h>
#include "Battery.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "adc_cali_schemes.h"
int Battery::detect() {

    ESP_ERROR_CHECK(adc_oneshot_read(adc1_handle, VBAT_ADC_CHAN, &adc_raw[0][0]));
    if (is_calibration) {
        ESP_ERROR_CHECK(adc_cali_raw_to_voltage(adc1_cali_chan0_handle, adc_raw[0][0], &voltage[0][0]));
    }

    int battery_voltage = voltage[0][0] * 2;
    printf("battery voltage: %d\n", battery_voltage);
    battery_voltage = battery_voltage_smoothing(battery_voltage);
    float voltage_percent = ((battery_voltage - 0) / (4096.0 - 0)) * 100.0 + 1;
    int battery_voltage_percent = (int)voltage_percent;
    static int battery_voltage_percent_last = 75;
    if (battery_voltage_percent_last != battery_voltage_percent) {
        // ESP_LOGW(TAG, "battery voltage percent: %d", battery_voltage_percent);
        Serial.printf("battery voltage percent: %d\r\b", battery_voltage_percent);
        battery_voltage_percent_last = battery_voltage_percent;
    }

    int vbat_charging_state = gpio_get_level(monitorPin);

    if (vbat_charging_state != last_vbat_charging_state) {
        last_vbat_charging_state = vbat_charging_state;
        if (vbat_charging_state == VBAT_CHARGING_LEVEL) {
            Serial.println("The battery starts charging.");
        } else {
            Serial.println("The battery is not charging.");
        }
    }

    percent = battery_voltage_percent;

    return battery_voltage_percent;
}

// 根据电压估算电池电量
int Battery::estimateCapacity(float voltage) {
    // 锂电池电压 - 电量映射表（示例，不同电池可能不同）
    const float voltageTable[] = {3.0, 3.2, 3.4, 3.6, 3.8, 4.0, 4.2};
    const int capacityTable[] = {0, 10, 30, 50, 70, 90, 100};
    int tableSize = sizeof(voltageTable) / sizeof(voltageTable[0]);
    for (int i = 0; i < tableSize - 1; i++) {
        if (voltage >= voltageTable[i] && voltage < voltageTable[i + 1]) {
            // 线性插值估算电量
            float slope = (capacityTable[i + 1] - capacityTable[i]) / (voltageTable[i + 1] - voltageTable[i]);
            return capacityTable[i] + slope * (voltage - voltageTable[i]);
        }
    }
    if (voltage < voltageTable[0]) {
        return 0;
    } else {
        return 100;
    }
}    

static const char *TAG = "power_management";



// static light_mode_t power_current_light_mode = LIGHT_MODE_SLEEP;

int Battery::battery_voltage_smoothing(int new_sample)
{
    static int battery_voltage_samples[ADC_SMOOTH_WINDOW_SIZE] = {0};
    static uint8_t battery_voltage_index = 0;
    static int battery_voltage_sum = 0;

    battery_voltage_sum -= battery_voltage_samples[battery_voltage_index];
    battery_voltage_samples[battery_voltage_index] = new_sample;
    battery_voltage_sum += new_sample;
    battery_voltage_index = (battery_voltage_index + 1) % ADC_SMOOTH_WINDOW_SIZE;

    return battery_voltage_sum / ADC_SMOOTH_WINDOW_SIZE;
}

/*---------------------------------------------------------------
        ADC Calibration
---------------------------------------------------------------*/
bool Battery::adc_calibration_init(adc_unit_t unit, adc_channel_t channel, adc_atten_t atten, adc_cali_handle_t *out_handle)
{
    adc_cali_handle_t handle = NULL;
    esp_err_t ret = ESP_FAIL;
    bool calibrated = false;

// #if ADC_CALI_SCHEME_CURVE_FITTING_SUPPORTED
    if (!calibrated) {
        ESP_LOGI(TAG, "calibration scheme version is %s", "Curve Fitting");
        adc_cali_curve_fitting_config_t cali_config = {
            .unit_id = unit,
            .chan = channel,
            .atten = atten,
            .bitwidth = ADC_BITWIDTH_DEFAULT,
        };
        ret = adc_cali_create_scheme_curve_fitting(&cali_config, &handle);
        if (ret == ESP_OK) {
            calibrated = true;
        }
    }
// #endif

#if ADC_CALI_SCHEME_LINE_FITTING_SUPPORTED
    if (!calibrated) {
        ESP_LOGI(TAG, "calibration scheme version is %s", "Line Fitting");
        adc_cali_line_fitting_config_t cali_config = {
            .unit_id = unit,
            .atten = atten,
            .bitwidth = ADC_BITWIDTH_DEFAULT,
        };
        ret = adc_cali_create_scheme_line_fitting(&cali_config, &handle);
        if (ret == ESP_OK) {
            calibrated = true;
        }
    }
#endif

    *out_handle = handle;
    if (ret == ESP_OK) {
        ESP_LOGI(TAG, "Calibration Success");
    } else if (ret == ESP_ERR_NOT_SUPPORTED || !calibrated) {
        ESP_LOGW(TAG, "eFuse not burnt, skip software calibration");
    } else {
        ESP_LOGE(TAG, "Invalid arg or no memory");
    }

    return calibrated;
}

void Battery::setup() {
    gpio_config_t vbat_monitor_io_config = {
        .pin_bit_mask = 1ULL << monitorPin,
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };
    gpio_config(&vbat_monitor_io_config);
    // static int last_vbat_charging_state = !VBAT_CHARGING_LEVEL;

    //-------------ADC1 Init---------------//
    init_config1 = {
        .unit_id = ADC_UNIT_1,
    };
    ESP_ERROR_CHECK(adc_oneshot_new_unit(&init_config1, &adc1_handle));

    //-------------ADC1 Config---------------//
    adc_oneshot_chan_cfg_t config = {
        .atten = ADC_ATTEN_DB_12,
        .bitwidth = ADC_BITWIDTH_12,

    };
    ESP_ERROR_CHECK(adc_oneshot_config_channel(adc1_handle, VBAT_ADC_CHAN, &config));
    Serial.println("init");

    // //-------------ADC1 Calibration Init---------------//
    // adc_cali_handle_t adc1_cali_chan0_handle = NULL;
    is_calibration = adc_calibration_init(ADC_UNIT_1, VBAT_ADC_CHAN, ADC_ATTEN_DB_12, &adc1_cali_chan0_handle);
}
