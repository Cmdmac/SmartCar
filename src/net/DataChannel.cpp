#include "DataChannel.h"
#include "src/net/Uri.h"
#include "src/common/Command.h"
#include "src/ibeacon/iBeacon.h"
#include "src/net/net.h"
// #include "QMC5883LCompass.h"

Net net;
iBeaconFinder finder;
// QMC5883LCompass compass;

void iBeaconTask() {
// Serial.println("scanIBeacons");
    // find();
    // Serial.println("report to server");
    vector<iBeacon> devices = finder.getDevices();

    Stark::Uri uri(WS_SERVER);
    char buffer[10];
    
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
    // compass.read();
    // Return Azimuth reading
    // int a = compass.getAzimuth() + 180;
    // std::string s = CommandBuilder::CreateCodeJson(CMD_REPORT_DIRECTION, a);
    // Serial.println(s.c_str());
    // net.ws().send(s.c_str());
    // Serial.print("A: ");
    // Serial.print(a);
    // Serial.println();
}

void reportTask(void* p) {
	while(1) {
        // compassTask(NULL);
        iBeaconTask();
        // compassTask();
        delay(1000);
	}
}

void startTasks() {
    
    finder.init();
    finder.startFind();
    // compass.init(PIN_COMPASS_SDA, PIN_COMPASS_SCL);
    xTaskCreatePinnedToCore(reportTask, "reportTask", 4096, NULL, 1, NULL, 0);
}
