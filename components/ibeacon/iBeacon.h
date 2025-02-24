#ifndef _IBEACON_H
#define _IBEACON_H
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <string>
#include <sstream>
#include <vector>
#include <Ticker.h>
using namespace std;

class iBeacon {
  public:
    string name;
    string address;
    int rssi;
    string toString() {
      std::stringstream ss;
    
      ss << "name=" << name.c_str() << ",addr=" << address.c_str() << ",rssi=" << rssi;
      return ss.str();
    }
};

class BleCallback : public BLEAdvertisedDeviceCallbacks {
  private:
    vector<iBeacon> devices;
    int len;

  public:
    BleCallback();
    void onResult(BLEAdvertisedDevice adv);
    vector<iBeacon> getIbeacons();
};

class iBeaconFinder {
    private:
        BLEScan* pBLEScan;
        BleCallback* pBleCallback;
        Ticker beaconTimer;
        
        static bool compareiBeacon(const iBeacon& p1, const iBeacon& p2) {
            return p1.rssi > p2.rssi;
        }

        // static void reportDelegate(iBeaconFinder *pvParameters) {
        //   // 将传入的参数转换为类的实例指针
        //   iBeaconFinder* instance = static_cast<iBeaconFinder*>(pvParameters);
        //   // 调用类的成员函数
        //   instance->reportTask();
        // }

        static void findDelegate(void *pvParameters) {
          iBeaconFinder* instance = static_cast<iBeaconFinder*>(pvParameters);
          while(1) {
            instance->find();
          }
        }

        // void reportTask();


    public:
        iBeaconFinder();
        void init();
        void find();
        void startFind();
        vector<iBeacon> getDevices();
        void loop();

};
#endif