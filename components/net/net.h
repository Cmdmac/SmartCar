#ifndef _NET_H_
#define _NET_H_
#include <Arduino.h>
#include <HTTPClient.h>
#include <ArduinoWebsockets.h>
#include <string>
#include <ArduinoJson.h>
#include <stdint.h>
#include <FS.h>
#include "Uri.h"
using namespace std;
using namespace websockets;
// using namespace Stark;
#define WS_SERVER "http://192.168.2.153:3000"

typedef std::function<void(int, JsonDocument&)> CommandCallback;
class Net {
    public:
        static string httpGet(string url);
        static void httpPostUploadFile(char* url, char* fileName, uint8_t* fileData, size_t fileDataLength);

        void setUpWifi();
        void setUpWebsocket(CommandCallback callback);
        WebsocketsClient& ws();
        void loop();

    private:
        void onMessageCallback(WebsocketsMessage message);
        void onEventsCallback(WebsocketsEvent event, String data);

        static void wsDelegate(void *pvParameters) {
          Net* instance = static_cast<Net*>(pvParameters);
          while(1) {
            instance->loop();
            delay(1);
          }
        }
        WebsocketsClient client;
        CommandCallback commandCallback;
};

#endif