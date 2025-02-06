#include <arduino.h>
// #include <WiFi.h>
// #include <string>
// #include <freertos/FreeRTOS.h>
// #include <freertos/task.h>
// #include "lib/common/Command.h"
#include "src/net/net.h"
// #include "lib/camera/Camera.h"
#include "src/led/Led.h"
#include "src/net/DataChannel.h"
// #include "Hall.h"
// #include "lib/speaker/Speaker.h"
// #include "lib/filewebserver/FileWebServer.h"
// #include <PCA9557.h>
#include "src/compass/QMC5883LCompass.h"

// using namespace std;
// using namespace websockets;

extern QMC5883LCompass compass;
extern Net net;
// extern iBeaconFinder finder;

// FileWebServer fileWebServer;

Led led;

// PCA9557 io(0x19, &Wire); // 0x19 for iFarm4G board
// #define D1_PIN (0)

// IRrecv irrecv(8);

// decode_results results;
// AnalogMic analogMic;
void setup() {
  Serial.begin(115200);
  Serial.println("setup");
  // Wire.begin(4, 5);

  // Configs D1, D2 to INPUT mode
  // io.pinMode(D1_PIN, OUTPUT);
  // io.digitalWrite(D1_PIN, 1);
  net.setUpWifi();
  // net.setUpWebsocket([](int cmd, JsonDocument& doc) {
  //   Serial.print(cmd);
  //   switch(cmd) {
  //     case CMD_SET_DIRECTION: {
  //         float d = doc["data"];
  //         Serial.print(" direction=");
  //         Serial.println(d);
  //         // car.direct(d);
  //       }
  //       break;
  //     case CMD_SET_SPEED: {
  //         float speed = doc["data"];
  //         Serial.print(" speed=");
  //         Serial.println(speed);
  //         // car.drive(speed);
  //       }
  //       break;
  //   }
  // });

  startTasks();

  // fileWebServer.setup();
  // analogMic.setup();
  // analogMic.record("/analogvoice.wav", 5);

  // mic.setup(38, 37, 36);
  // mic.recordWav("", 20, SAMPLE_RATE, SAMPLE_BITS);

  // camera.setUp();
  // camera.startStreamServer();

  // speaker.setup();
  // speaker.play("http://192.168.2.153:4000/voice.mp3");

  
}


void loop() {
}

