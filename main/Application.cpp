#include "Application.h"
#include "Config.h"
#include "Uri.h"
#include "Command.h"


void Application::setup() {
    // Serial.println("Application::setup");
    // setupBeacon();
    setUpWifi();
    setupBeacon();   
    setupCompass();
}

void Application::setUpWifi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.print("Connecting to WiFi ");
  while (WiFi.status()!= WL_CONNECTED) {
    Serial.print(WiFi.status());
    delay(1000);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Connected to WiFi localIP: ");
  Serial.println(WiFi.localIP());
}

void Application::setupBeacon() {
    Serial.println("Application::setupBeacon");
    finder.init();
    finder.startFind();
    xTaskCreatePinnedToCore(beaconDelegate, "beaconTask", 4096, NULL, 5, NULL, 0);
}

void Application::setupCompass() {
    Serial.println("Application::setupCompass");
    compass.init(4, 5);
    xTaskCreatePinnedToCore(compaseDelegate, "compassTask", 4096, NULL, 5, NULL, 0);
}

void Application::reportLocation(Protocol net) {
    // Serial.println("Application::setupBeacon");
    // beacon.setup();
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


void Application::compassTask() {

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

void Application::scanI2CDevices() {
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