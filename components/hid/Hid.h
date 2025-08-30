
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <BLEHIDDevice.h>
#include <map>
#include "BLERemoteDescriptor.h"
#include <esp_gattc_api.h>
#include <esp_bt_defs.h>          // 补充必要的蓝牙类型定义
#include <esp_gatt_common_api.h>  // 包含GATT相关类型
// // 定义HID服务的UUID
// #define HID_SERVICE_UUID "1812" // 标准HID服务UUID

// // 定义HID报告特征UUID
// #define REPORT_MAP_UUID "2A4B"
// #define HID_INFORMATION_UUID "2A4A"
// #define REPORT_UUID "2A4D"
// #define HID_CONTROL_POINT_UUID "2A4C"
// HID 标准特征值 UUID（完整格式，适配 3.2.0 版本）
#define HID_SERVICE_UUID        "00001812-0000-1000-8000-00805f9b34fb" // HID 服务 UUID（必须先找到该服务）
#define REPORT_MAP_UUID         "00002a4b-0000-1000-8000-00805f9b34fb" // 报告映射（数据格式说明书）
#define HID_INFORMATION_UUID    "00002a4a-0000-1000-8000-00805f9b34fb" // HID 信息（设备类型）
#define REPORT_UUID             "00002a4d-0000-1000-8000-00805f9b34fb" // 核心：数据传输通道
#define HID_CONTROL_POINT_UUID  "00002a4c-0000-1000-8000-00805f9b34fb" // 控制点（主机控制手柄）
#define CCCD_UUID               "00002902-0000-1000-8000-00805f9b34fb" // 通知控制描述符（启用通知用）

// 全局变量
BLEScan* pBLEScan;
BLEClient* pClient;
bool deviceFound = false;
BLEAdvertisedDevice* myDevice;
bool connected = false;

// 手柄数据存储
uint8_t hidReport[8]; // 根据您的手柄报告长度调整

// 回调类：处理发现的设备
class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    if(advertisedDevice.getName().length() == 0) {
      return;
    }
    
    Serial.printf("device: %s-%s-%d\n", advertisedDevice.getName().c_str(), advertisedDevice.getServiceUUID().toString().c_str(), advertisedDevice.isAdvertisingService(BLEUUID(HID_SERVICE_UUID)));
    
    // 检查设备是否提供HID服务
    if (advertisedDevice.getName() == "GameSir-Nova 2 Lite_6e" && 
        advertisedDevice.haveServiceUUID() && 
        advertisedDevice.isAdvertisingService(BLEUUID(HID_SERVICE_UUID))) {
      
      Serial.print("Found HID device: ");
      Serial.println(advertisedDevice.toString().c_str());
      
      // 停止扫描
      pBLEScan->stop();
      
      // 保存设备信息
      myDevice = new BLEAdvertisedDevice(advertisedDevice);
      deviceFound = true;
    }
  }
};

// 连接回调
class MyClientCallback : public BLEClientCallbacks {
    void onConnect(BLEClient* pclient) {
        // deviceConnected = true;
        Serial.println("设备已连接");
    }

    void onDisconnect(BLEClient* pclient) {
        // deviceConnected = false;
        Serial.println("设备已断开连接");
        // pRemoteHIDCharacteristic = nullptr;
    }
};

// 回调函数：处理通知
static void notifyCallback(BLERemoteCharacteristic* pBLERemoteCharacteristic, 
                          uint8_t* pData, size_t length, bool isNotify) {
  // 复制数据到hidReport
  memcpy(hidReport, pData, (length < sizeof(hidReport)) ? length : sizeof(hidReport));
  
  // 打印接收到的数据
  Serial.print("Received HID Report: ");
  for (int i = 0; i < length; i++) {
    Serial.print(pData[i], HEX);
    Serial.print(" ");
  }
  Serial.println();
  
  // 解析手柄数据
  // parseHIDData(pData, length);
}

// 解析HID数据
void parseHIDData(uint8_t* data, size_t length) {
  // 这里需要根据您的手柄HID报告描述符来解析数据
  // 以下是一个通用示例，您需要根据实际手柄调整
  
  if (length >= 8) {
    // 假设数据格式为: [按钮1, 按钮2, 左摇杆X, 左摇杆Y, 右摇杆X, 右摇杆Y, 左扳机, 右扳机]
    uint8_t button1 = data[0];
    uint8_t button2 = data[1];
    int8_t leftStickX = data[2];
    int8_t leftStickY = data[3];
    int8_t rightStickX = data[4];
    int8_t rightStickY = data[5];
    uint8_t leftTrigger = data[6];
    uint8_t rightTrigger = data[7];
    
    Serial.print("Buttons: ");
    Serial.print(button1, BIN);
    Serial.print(" ");
    Serial.print(button2, BIN);
    Serial.print(" | Sticks: LX=");
    Serial.print(leftStickX);
    Serial.print(" LY=");
    Serial.print(leftStickY);
    Serial.print(" RX=");
    Serial.print(rightStickX);
    Serial.print(" RY=");
    Serial.print(rightStickY);
    Serial.print(" | Triggers: L=");
    Serial.print(leftTrigger);
    Serial.print(" R=");
    Serial.println(rightTrigger);
  }
}

// 3. 安全回调（打印加密状态）
class MySecurityCallback : public BLESecurityCallbacks {
    // void onAuthenticationComplete(esp_ble_auth_cmpl_t result) {
    //     isEncrypted = (result.success == true);
    //     Serial.printf("加密认证结果: %s\n", isEncrypted ? "成功" : "失败");
    //     if (!isEncrypted) {
    //         Serial.println("警告：未加密可能导致无法接收数据，尝试重置手柄配对信息");
    //     }
    // }
    // bool onConfirmPIN(uint32_t pin) {
    //     Serial.printf("需要配对PIN: %d（直接确认）\n", pin);
    //     return true; // 自动确认PIN（部分手柄需要）
    // }
    // 必须实现所有纯虚函数
    uint32_t onPassKeyRequest() override {
        Serial.println("请求配对密钥，返回 000000");
        return 000000;
    }

    void onPassKeyNotify(uint32_t pass_key) override {
        // Serial.printf("配对密钥: %d\n", pass_key);
    }

    bool onSecurityRequest() override {
        Serial.println("允许安全请求");
        return true;
    }

    bool onConfirmPIN(uint32_t pin) override {
        // Serial.printf("确认PIN码: %d\n", pin);
        return true; // 确认PIN码有效
    }

    void onAuthenticationComplete(esp_ble_auth_cmpl_t auth_cmpl) override {
        if (auth_cmpl.success) {
            Serial.println("认证成功");
        } else {
            Serial.printf("认证失败，错误码: %d\n", auth_cmpl.fail_reason);
        }
    }
};

void setupSecurity() {
    BLESecurity* pSecurity = new BLESecurity();
    pSecurity->setAuthenticationMode(ESP_LE_AUTH_REQ_SC_BOND); // 启用安全连接
    pSecurity->setCapability(ESP_IO_CAP_NONE); // 无输入输出能力（手柄通常无需PIN）
    pSecurity->setKeySize(16); // 加密密钥长度
}
// // 启用通知（适配3.2.0版本）
// void enableNotification(BLERemoteCharacteristic* pChar) {
//     if (!pChar) return;

//     // 获取特征值属性（使用兼容方法）
//     uint16_t properties = getCharacteristicProperties(pChar);
//     if (!(properties & ESP_GATT_CHAR_PROP_BIT_NOTIFY)) {
//         Serial.println("REPORT特征值不支持通知，无法接收数据");
//         return;
//     }

//     // 注册数据回调
//     pChar->registerForNotify(hidDataCallback);

//     // 写入CCCD启用通知
//     BLERemoteDescriptor* pCCCD = pChar->getDescriptor(BLEUUID(CCCD_UUID));
//     if (!pCCCD) {
//         Serial.println("未找到CCCD描述符");
//         return;
//     }
//     uint8_t enableData[2] = {0x01, 0x00}; // 启用通知的标准值
//     pCCCD->writeValue(enableData, 2);
//     Serial.println("已启用通知，等待数据...");
// }

void checkCCCDValue(BLERemoteDescriptor* pCCCD) {
    if (!pCCCD) return;
    String cccdValue = pCCCD->readValue();
    Serial.print("CCCD值（字节顺序）: ");
    for (uint8_t b : cccdValue) {
        Serial.printf("%02X ", b);
    }

    // 解析实际功能（考虑字节顺序）
    if (cccdValue.length() == 2) {
        uint16_t cccd = (cccdValue[1] << 8) | cccdValue[0]; // 转换为标准16位值
        if (cccd & 0x0001) Serial.println("→ 已启用通知");
        if (cccd & 0x0002) Serial.println("→ 已启用指示");
    } else {
        Serial.println("→ 无效的CCCD值");
    }
}

// 连接到HID设备
bool connectToServer() {
  Serial.print("Connecting to ");
  Serial.println(myDevice->getAddress().toString().c_str());
  
  // 创建客户端
  pClient = BLEDevice::createClient();
  Serial.println(" - Created client");
  
  pClient->setClientCallbacks(new MyClientCallback());

  // 连接到设备
  if (!pClient->connect(myDevice)) {
    Serial.println(" - Connection failed");
    return false;
  }
  Serial.println(" - Connected to server");
  
  // 获取HID服务
  BLERemoteService* pHidService = pClient->getService(BLEUUID(HID_SERVICE_UUID));
  if (pHidService == nullptr) {
    Serial.println(" - Failed to find HID service");
    pClient->disconnect();
    return false;
  }
  Serial.println(" - Found HID service");
  
  // 获取报告特征
  // BLERemoteCharacteristic* pReportCharacteristic = pHidService->getCharacteristic(BLEUUID(REPORT_UUID));
std::map<std::string, BLERemoteCharacteristic*>* cs = pHidService->getCharacteristics();
for (std::map<std::string, BLERemoteCharacteristic*>::const_iterator it = cs->begin(); it != cs->end(); ++it) {  // 遍历服务的Characteristics
  BLERemoteCharacteristic* pChara = it->second;
  if (pChara == nullptr) {
    Serial.println(" - Failed to find report characteristic");
    pClient->disconnect();
    return false;
  }

  // 1. 获取特征值的UUID
  BLEUUID charUUID = pChara->getUUID();
  Serial.printf("\n===== 特征值信息 (UUID: %s) =====\n", charUUID.toString().c_str());

  // 2. 获取特征值的属性（可读、可写、通知等）
  std::map<std::string, BLERemoteDescriptor*>* descriptors = pChara->getDescriptors();
  for (std::map<std::string, BLERemoteDescriptor*>::const_iterator it = descriptors->begin(); it != descriptors->end(); ++it) {
    // 获取键（UUID 字符串）和值（描述符指针）
    const std::string& uuidStr = it->first;
    BLERemoteDescriptor* pDescriptor = it->second;
    // 打印描述符信息`
    Serial.printf("键（UUID）: %s\n", uuidStr.c_str());
  }
  // 4. 启用 REPORT（2A4D）的通知：让手柄主动推送数据
  //   enableNotification(pReportCharacteristic);

  Serial.println(" - Found report characteristic");

  // 注册通知回调
  if (pChara->canNotify()) {
    pChara->registerForNotify(notifyCallback);
    Serial.println(" - Registered for notifications");

    // // 获取CCCD并写入
    BLERemoteDescriptor* pCCCD = pChara->getDescriptor(BLEUUID(CCCD_UUID));
    // if (pCCCD) {
    //   // uint8_t enableData[2] = {0x03, 0x00}; // 小端序：03 00 → 0x0003

    //   uint8_t enableData[2] = { 0x00, 0x00 };
    //   pCCCD->writeValue(enableData, 2);
    //   delay(500);             // 等待写入生效
      checkCCCDValue(pCCCD);  // 检查写入结果
    // } else {
    //   Serial.println("未找到CCCD描述符，无法启用通知");
    // }
  }
}
  
  connected = true;
  return true;
}

void setup() {
  Serial.begin(115200);
  Serial.println("ESP32 HID Host - Scanning for HID devices...");
  
  // // 初始化BLE
  // BLEDevice::init("");

  // setupSecurity();

  // // 创建扫描对象
  // pBLEScan = BLEDevice::getScan();
  // pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  // pBLEScan->setActiveScan(true);
  // pBLEScan->setInterval(10);
  // pBLEScan->setWindow(99);
}

void loop() {
  // if (!connected) {
  //   if (!deviceFound) {
  //     // 扫描设备
  //     Serial.println("Scanning for HID devices...");
  //     pBLEScan->start(5, false);
  //     Serial.println("Scan done!");
  //     pBLEScan->clearResults();
  //   } else {
  //     // 尝试连接
  //     if (connectToServer()) {
  //       Serial.println("Connected to HID device!");
  //     } else {
  //       Serial.println("Connection failed, restarting scan...");
  //       deviceFound = false;
  //       delay(1000);
  //     }
  //   }
  // } else {
  //   // 已连接，处理数据
  //   delay(100);
    
  //   // 可以添加其他处理逻辑，如发送数据到其他设备
  // }
}