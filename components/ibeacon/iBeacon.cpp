#include "iBeacon.h"
#include <Arduino.h>

BleCallback::BleCallback() : len(0) {
      
}

void BleCallback::onResult(BLEAdvertisedDevice adv) {
    // Serial.println("设备名称: " + adv.getName() + "MAC 地址: " + adv.getAddress().toString() + "RSSI: " + new String(adv.getRSSI()));
    const char*name = adv.getName().c_str();
    // Serial.println(adv.getName().c_str());
    if (strcmp(name, "Holy-IOT") == 0) {
        // devices[len++] = beacon;
        string address = adv.getAddress().toString().c_str();
        bool isFound = false;
        for (std::vector<iBeacon>::iterator it = devices.begin(); it!= devices.end(); ++it) {
            if (it->address.compare(address) == 0) {
                it->rssi = adv.getRSSI();
                // Serial.print("found:");
                // Serial.print(adv.getAddress().toString().c_str());
                // Serial.println(adv.getRSSI());
                isFound = true;
                break;
            }
        }
        if (!isFound) {
            iBeacon beacon;
            beacon.name = adv.getName().c_str();
            beacon.address = adv.getAddress().toString().c_str();
            beacon.rssi = adv.getRSSI();
            devices.push_back(beacon);
            Serial.print("add beacon ");
            Serial.println(beacon.toString().c_str());
        }
    }
}

vector<iBeacon> BleCallback::getIbeacons() {
    return devices;
}

iBeaconFinder::iBeaconFinder():pBleCallback(NULL) {

}

void iBeaconFinder::init() {
    BLEDevice::init("");
    pBLEScan = BLEDevice::getScan();
}

void iBeaconFinder::find() {
    pBLEScan->clearResults();
    if (pBleCallback == NULL) {
        pBleCallback = new BleCallback();
    }
    pBLEScan->setAdvertisedDeviceCallbacks(pBleCallback);
    pBLEScan->setActiveScan(true);
    pBLEScan->start(8, false);  
}

// void iBeaconFinder::reportTask() {
//     // Serial.println("scanIBeacons");
//     // find();
//     Serial.println("report to server");
//     vector<iBeacon> devices = getDevices();

//     Uri uri(WS_SERVER);
//     char buffer[10];
    
//     uri.appendPath("locate");
//     for (int i = 0; i < devices.size(); i++) {
//         sprintf(buffer, "mac%d", i + 1);
//         uri.appendQuery(buffer ,devices[i].address);
//         ::memset(buffer, 0, 10);
//         sprintf(buffer, "rssi%d", i + 1);
//         uri.appendQuery(buffer , devices[i].rssi);
//         ::memset(buffer, 0, 10);
//     }
//     Serial.println(uri.toString().c_str());
//     Net::httpGet(uri.toString());
// }

void iBeaconFinder::startFind() {
    // beaconTimer.attach_ms(1000, &iBeaconFinder::reportDelegate, this);
    xTaskCreatePinnedToCore(&iBeaconFinder::findDelegate, "findBleTask", 4096, this, 1, NULL, 0);
}

vector<iBeacon> iBeaconFinder::getDevices() {
    if (pBleCallback != NULL) {
        vector<iBeacon> devices = pBleCallback->getIbeacons();
        std::sort(devices.begin(), devices.end(), iBeaconFinder::compareiBeacon);
        return devices;
    } else {
        vector<iBeacon> devices;
        return devices;
    }
}

void iBeaconFinder::loop() {
    
}