#include <Arduino.h>
#include "Battery.h"

int Battery::detect() {

    const float R1 = 100000; // 分压电阻 R1 的阻值，单位：欧姆
    const float R2 = 100000; // 分压电阻 R2 的阻值，单位：欧姆
    const float refVoltage = 3.3; // ESP32 - S3 的参考电压，单位：伏特
    const int adcResolution = 4095; // ESP32 - S3 的 ADC 分辨率（12 位）

    pinMode(detectPin, INPUT);
    int adcValue = analogRead(detectPin); // 读取 ADC 值
    float adcVoltage = adcValue * (refVoltage / adcResolution); // 将 ADC 值转换为电压
    float batteryVoltage = adcVoltage * ((R1 + R2) / R2); // 根据分压公式计算电池电压

    int batteryCapacity = estimateCapacity(batteryVoltage); // 估算电池电量

    Serial.print("adcValue:");
    Serial.print(adcValue);
    Serial.print(" Battery Voltage: ");
    Serial.print(batteryVoltage);
    Serial.print(" V, Battery Capacity: ");
    Serial.print(batteryCapacity);
    Serial.println(" %");

    this->percent = batteryVoltage;

    return batteryCapacity;
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