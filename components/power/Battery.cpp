#include <Arduino.h>
#include "Battery.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "adc_cali_schemes.h"

static const char *TAG = "Battery";

void Battery::initializeADC() {
    adc_oneshot_unit_init_cfg_t init_config1 = {
        .unit_id = ADC_UNIT_1
    };
    ESP_ERROR_CHECK(adc_oneshot_new_unit(&init_config1, &adc1_handle));

    adc_oneshot_chan_cfg_t chan_config = {
        .atten = ADC_ATTEN,
        .bitwidth = ADC_WIDTH,
    };
    ESP_ERROR_CHECK(adc_oneshot_config_channel(adc1_handle, VBAT_ADC_CHANNEL, &chan_config));

    adc_cali_handle_t handle = NULL;
    esp_err_t ret = ESP_FAIL;

#if ADC_CALI_SCHEME_CURVE_FITTING_SUPPORTED
    adc_cali_curve_fitting_config_t cali_config = {
        .unit_id = ADC_UNIT_1,
        .atten = ADC_ATTEN,
        .bitwidth = ADC_WIDTH,
    };
    ret = adc_cali_create_scheme_curve_fitting(&cali_config, &handle);
    if (ret == ESP_OK) {
        do_calibration = true;
        adc1_cali_handle = handle;
        ESP_LOGI(TAG, "ADC Curve Fitting calibration succeeded");
    }
#endif // ADC_CALI_SCHEME_CURVE_FITTING_SUPPORTED
}

int Battery::detect() {

    if (!adc1_handle) {
        initializeADC();
    }

    int raw_value = 0;
    int voltage = 0;

    ESP_ERROR_CHECK(adc_oneshot_read(adc1_handle, VBAT_ADC_CHANNEL, &raw_value));

    if (do_calibration) {
        ESP_ERROR_CHECK(adc_cali_raw_to_voltage(adc1_cali_handle, raw_value, &voltage));
        voltage = voltage * 3 / 2; // compensate for voltage divider
        // ESP_LOGI(TAG, "Calibrated voltage: %d mV", voltage);
    } else {
        // ESP_LOGI(TAG, "Raw ADC value: %d", raw_value);
        voltage = raw_value;
    }

    voltage = smoothVoltageEWMA(voltage);

    voltage = voltage < EMPTY_BATTERY_VOLTAGE ? EMPTY_BATTERY_VOLTAGE : voltage;
    voltage = voltage > FULL_BATTERY_VOLTAGE ? FULL_BATTERY_VOLTAGE : voltage;

    // 计算电量百分比
    int level = (voltage - EMPTY_BATTERY_VOLTAGE) * 100 / (FULL_BATTERY_VOLTAGE - EMPTY_BATTERY_VOLTAGE);

    // charging = gpio_get_level(MCU_VCC_CTL);
    ESP_LOGI(TAG, "voltage:%dmv, Level: %d%%", voltage, level);
    return (int)level;
}

void Battery::setup() {
    // gpio_config_t vbat_monitor_io_config = {
    //     .pin_bit_mask = 1ULL << monitorPin,
    //     .mode = GPIO_MODE_INPUT,
    //     .pull_up_en = GPIO_PULLUP_ENABLE,
    //     .pull_down_en = GPIO_PULLDOWN_DISABLE,
    //     .intr_type = GPIO_INTR_DISABLE
    // };
    initializeADC();
}
