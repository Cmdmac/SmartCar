#ifndef _NET_H_
#define _NET_H_
#include <Arduino.h>
#include <HTTPClient.h>
#include <ArduinoWebsockets.h>
#include <string>
#include <ArduinoJson.h>
#include <stdint.h>
#include <FS.h>
// #include "Uri.h"
using namespace std;
using namespace websockets;
// using namespace Stark;
#define WS_SERVER "http://192.168.2.153:3000"

typedef std::function<void(int, JsonDocument&)> CommandCallback;
class Net {
    public:
        static string httpGet(string url);
        static void httpPost(string url, char* fileData, int fileDataLength);
        void setUpWifi();
        void setUpWebsocket(CommandCallback callback);
        WebsocketsClient& ws();

    private:
        void loop();
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
/*
class NetHttpFile : public fs::File {
public:
    NetHttpFile(const char* url, const string fileName) : isConnected(false), uri(url), fileName(fileName) {

      
    }

    size_t write(const uint8_t *buf, size_t size) override {
      if (!isConnected) {
        // 连接到服务器
        WiFiClient client;
        if (!client.connect(uri.getHost().c_str(), 80)) {
            Serial.println("Connection failed");
            // file.close();
            return 0;
        }

        // 构建HTTP请求头
        string header = "POST " + string(uri.getPath()) + " HTTP/1.1\r\n";
        header += "Host: " + string(uri.getHost()) + "\r\n";
        header += "Content-Type: multipart/form-data; boundary=----WebKitFormBoundary7MA4YWxkTrZu0gW\r\n";
        header += "Content-Length: " + string(String(fileLength).c_str()) + "\r\n"; // 估计总长度
        header += "Connection: close\r\n\r\n";
        header += "------WebKitFormBoundary7MA4YWxkTrZu0gW\r\n";
        header += "Content-Disposition: form-data; name=\"file\"; filename=\"" + string(fileName) + "\"\r\n";
        header += "Content-Type: text/plain\r\n\r\n";

        // 发送HTTP请求头
        client.print(header.c_str());
      }
      // int httpResponseCode = http->POST((uint8_t*)buf, size);
      // if (httpResponseCode > 0) {
      //     Serial.print("HTTP Response code: ");
      //     Serial.println(httpResponseCode);
      // } else {
      //     Serial.print("Error on sending POST: ");
      //     Serial.println(httpResponseCode);
      // }
      // return httpResponseCode;
      return size;
    }

    void close() {
      File::close();
    }

public:
  bool isConnected;
  Stark::Uri uri;
  string fileName;
  long fileLength;
};
*/
#endif