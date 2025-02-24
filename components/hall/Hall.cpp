#include "Hall.h"
#include <Arduino.h>
#include <Ticker.h>
#include "net.h"
#include <sstream>

Ticker ticker;

static const int MG310_PPR = 13;
// Ticker ticker;
#define ENCODER1_PIN_A 12
#define ENCODER1_PIN_B 13

// 用于存储编码器计数
volatile long ENCODER1_COUNT = 0, ENCODER2_COUNT = 0;
// 变量用于存储电机转速（单位：转每分钟，RPM）
int LAST_ENCODER1_A_STATE = LOW, LAST_ENCODER1_B_STATE = LOW;
float RPM_1, RPM_2;
// 编码器每转的脉冲数（根据你的编码器规格设置）
int PPR_1 = MG310_PPR, PPR_2 = MG310_PPR;

// 中断服务函数，用于处理A相的变化
void encodeAISR() {
    int currentAState = digitalRead(ENCODER1_PIN_A);
    int currentBState = digitalRead(ENCODER1_PIN_B);
    // // 如果A相发生了上升沿变化
    if (currentAState == HIGH && LAST_ENCODER1_A_STATE == LOW) {
        // 根据B相的状态确定转动方向
        // Serial.println("do");

        // if (currentBState == LOW) {
            ENCODER1_COUNT++;
    //     } else {
    //         ENCODER1_COUNT--;
        // }
    }

    LAST_ENCODER1_A_STATE = currentAState;
}

// 定时执行的函数，用于计算转速等数据处理
void processEncoderData() {
    // 根据脉冲计数和时间间隔计算转速
    // Serial.print("ENCODER1_COUNT: ");
    Serial.println(LAST_ENCODER1_A_STATE);
    RPM_1 = (float)ENCODER1_COUNT * 60 / PPR_1;

    // 重置编码器计数
    ENCODER1_COUNT = 0;

    // Serial.print("RPM: ");
    // Serial.println(RPM_1);
}

void test2() {
    //  Serial.println("test2");
}

void initHall() {
    // 设置编码器引脚为输入模式
    pinMode(ENCODER1_PIN_A, INPUT);
    pinMode(ENCODER1_PIN_B, INPUT);

    // 为A相引脚的上升沿变化绑定中断服务函数
    attachInterrupt(digitalPinToInterrupt(ENCODER1_PIN_A), encodeAISR, RISING);

    // 设置Ticker，每1秒执行一次processEncoderData函数
    ticker.attach_ms(1000, processEncoderData);  // 打开定时器中断
}
