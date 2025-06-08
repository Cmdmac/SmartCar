#include "DataChannel.h"
#include "Uri.h"
#include "Command.h"
#include "iBeacon.h"
#include "Net.h"
#include "QMC5883LCompass.h"

  Net net;
iBeaconFinder finder;
QMC5883LCompass compass;

void iBeaconTask() {
// Serial.println("scanIBeacons");
    // find();
    // Serial.println("report to server");
    vector<iBeacon> devices = finder.getDevices();

    Stark::Uri uri(WS_SERVER);
    char buffer[32];
    
    uri.appendPath("locate");

    JsonDocument doc;
    for (int i = 0; i < devices.size(); i++) {
        sprintf(buffer, "mac%d", i + 1);
        doc[buffer] = devices[i].address;
        uri.appendQuery(buffer ,devices[i].address);
        ::memset(buffer, 0, 10);
        sprintf(buffer, "rssi%d", i + 1);
        doc[buffer] = devices[i].rssi;
        uri.appendQuery(buffer , devices[i].rssi);
        ::memset(buffer, 0, 10);
    }


    std::string str;
    serializeJson(doc, str);
    // Serial.println(uri.toString().c_str());
    std::string s = CommandBuilder::CreateCodeJson(CMD_REPORT_LOCATION, str);
    // Net::httpGet(uri.toString());
    Serial.println(s.c_str());
    net.ws().send(s.c_str());
}

void compassTask() {

    // Read compass values
    compass.read();
    // Return Azimuth reading
    int a = compass.getAzimuth() + 180;
    std::string s = CommandBuilder::CreateCodeJson(CMD_REPORT_DIRECTION, a);
    Serial.println(s.c_str());
    net.ws().send(s.c_str());
    Serial.print("A: ");
    Serial.print(a);
    Serial.println();
}

void reportTask(void* p) {
	while(1) {
        // compassTask(NULL);
        // iBeaconTask();
        compassTask();
        delay(1000);
	}
}

void startTasks() {
    
    finder.init();
    finder.startFind();
    compass.init(4, 5);
    xTaskCreatePinnedToCore(reportTask, "reportTask", 4096, NULL, 1, NULL, 0);
}

void scanI2CDevices() {
  Serial.println("\nI2C Scanner");
  Serial.println("Scanning...");

  uint8_t error, address;
  int nDevices;

  nDevices = 0;
  for (address = 1; address < 127; address++) {
    // 尝试连接到当前地址
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0) {
      Serial.print("I2C device found at address 0x");
      if (address < 16) {
        Serial.print("0");
      }
      ESP_LOGI("I2CScanner", "Device found at address 0x%02X", address);
      Serial.println(address, HEX);
      nDevices++;
    } else if (error == 4) {
      ESP_LOGI("I2CScanner", "Unknown error at address 0x%02X", address);

      Serial.print("Unknown error at address 0x");
      if (address < 16) {
        Serial.print("0");
      }
      Serial.println(address, HEX);
    } else {
      // ESP_LOGI("I2CScanner", "i2c error=%d\r\n", error);
      Serial.printf("i2c error=%d\r\n", error);
    }
  }
  if (nDevices == 0) {
    ESP_LOGI("I2CScanner", "No I2C devices found");
    Serial.println("No I2C devices found\n");
  } else {
    Serial.println("done\n");
  }
}

// #include <stdio.h>
// #include "driver/i2c.h"
// #include "esp_log.h"
// #include "driver/gpio.h"

// #define I2C_MASTER_SCL_IO           GPIO_NUM_13          // SCL引脚
// #define I2C_MASTER_SDA_IO           GPIO_NUM_12          // SDA引脚
// #define I2C_MASTER_NUM              I2C_NUM_0   // I2C控制器编号
// #define I2C_MASTER_FREQ_HZ          100000      // I2C时钟频率(100kHz)

// static const char *TAG = "i2c-scanner";

// /**
//  * @brief 扫描I2C总线上的设备
//  */
// void i2c_scanner(void)
// {
//  // 配置I2C参数
//     i2c_config_t conf = {
//         .mode = I2C_MODE_MASTER,
//         .sda_io_num = I2C_MASTER_SDA_IO,
//         .scl_io_num = I2C_MASTER_SCL_IO,
//         .sda_pullup_en = GPIO_PULLUP_ENABLE,
//         .scl_pullup_en = GPIO_PULLUP_ENABLE,
//         .master = {
//             .clk_speed = I2C_MASTER_FREQ_HZ,
//         },
//     };
    
//     // 配置I2C引脚
//     gpio_set_direction(I2C_MASTER_SDA_IO, GPIO_MODE_INPUT_OUTPUT_OD);
//     gpio_set_direction(I2C_MASTER_SCL_IO, GPIO_MODE_INPUT_OUTPUT_OD);
    
//     // 安装I2C驱动
//     ESP_ERROR_CHECK(i2c_param_config(I2C_MASTER_NUM, &conf) || 
//            i2c_driver_install(I2C_MASTER_NUM, conf.mode, 0, 0, 0));


//     ESP_LOGI(TAG, "Scanning I2C bus...");

//     uint8_t device_count = 0;
//     for (uint8_t address = 1; address < 127; address++) {
//         i2c_cmd_handle_t cmd = i2c_cmd_link_create();
//         i2c_master_start(cmd);
//         i2c_master_write_byte(cmd, (address << 1) | I2C_MASTER_WRITE, true);
//         i2c_master_stop(cmd);
        
//         esp_err_t err = i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, 1000 / portTICK_PERIOD_MS);
        
//         if (err == ESP_OK) {
//             ESP_LOGI(TAG, "Found I2C device at address 0x%02X", address);
//             device_count++;
//         } else if (err == ESP_ERR_TIMEOUT) {
//             ESP_LOGE(TAG, "I2C bus timeout");
//         }
        
//         i2c_cmd_link_delete(cmd);
//     }

//     if (device_count == 0) {
//         ESP_LOGW(TAG, "No I2C devices found");
//     } else {
//         ESP_LOGI(TAG, "Scan complete: %d device(s) found", device_count);
//     }

//         // 释放I2C资源
//     i2c_driver_delete(I2C_MASTER_NUM);
// }