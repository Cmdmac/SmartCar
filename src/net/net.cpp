#include "net.h"
#include <ArduinoJson.h>
#include <Base64.h>
string Net::httpGet(string url) {
    HTTPClient http;
    http.begin(url.c_str());  // 替换为您要请求的 URL

    int httpCode = http.GET();

    if (httpCode > 0) {
      if (httpCode == HTTP_CODE_OK) {
        String payload = http.getString();
        // Serial.println("Response:");
        // Serial.println(payload);
        return payload.c_str();
      }
    } else {
      Serial.printf("HTTP request failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
    return string();
}

void Net::httpPostUploadFile(char* url, char* fileName, uint8_t* fileData, size_t fileDataLength) {
  const char* boundary = "---------------------------9009C6232A16615E667";

  // 创建 WiFiClient 实例
  WiFiClient client;
  Stark::Uri uri = Stark::Uri(url);
  if (!client.connect(uri.getHost().c_str(), uri.getPort())) {
    Serial.println("Connection to server failed");
    return;
  }

  // 获取文件大小
  size_t fileSize = fileDataLength;//file.size();

  // 构建 multipart/form-data 请求头
  client.print("POST ");
  client.print(uri.getPath().c_str());
  client.println(" HTTP/1.1");
  client.print("Host: ");
  client.print(uri.getHost().c_str());
  client.print(":");
  client.println(uri.getPort());
  client.print("Content-Type: multipart/form-data; boundary=");
  client.println(boundary);
  client.print("Content-Length: ");
  // 计算请求体总长度
  size_t totalLength =
    strlen(boundary) + 4 + // 起始分隔符
    strlen("Content-Disposition: form-data; name=\"file\"; filename=\"test.wav\"\r\n") +
    strlen("Content-Type: audio/wav\r\n\r\n") +
    fileSize +
    strlen("\r\n") +
    strlen(boundary) + 6; // 结束分隔符
  client.println(totalLength);
  client.println();

  // 发送起始分隔符
  client.print("--");
  client.println(boundary);
  // 发送文件信息
  client.println("Content-Disposition: form-data; name=\"file\"; filename=\"test.wav\"");
  client.println("Content-Type: audio/wav");
  client.println();

  // 发送文件内容
  // while (file.available()) {
    client.write(fileData, fileDataLength);
  // }
  // file.close();

  // 发送结束分隔符
  client.print("\r\n--");
  client.print(boundary);
  client.println("--");

  // 等待服务器响应
  while (client.connected()) {
    if (client.available()) {
      String line = client.readStringUntil('\n');
      Serial.println(line);
    }
  }

  // 关闭连接
  client.stop();
}

const char* ssid     = "Stark";  // 替换为您的 Wi-Fi 网络名称
const char* password = "fengzhiping,1101";  // 替换为您的 Wi-Fi 密码
const char* websockets_url = "ws://192.168.2.153:3000/mobile/hub"; //Enter server adress


void Net::onMessageCallback(WebsocketsMessage message) {
    // Serial.print("Got Message: ");
    String str = message.data();
    // String str = message.data().substring(2, message.data().length() - 1);
    // Serial.println(str);
    JsonDocument doc;
    deserializeJson(doc, str.c_str());
    int cmd = doc["command"];
    if (commandCallback != NULL) {
        commandCallback(cmd, doc);
    }
}

void Net::onEventsCallback(WebsocketsEvent event, String data) {
    if(event == WebsocketsEvent::ConnectionOpened) {
        Serial.println("Connnection Opened");
    } else if(event == WebsocketsEvent::ConnectionClosed) {
        Serial.println("Connnection Closed");
    } else if(event == WebsocketsEvent::GotPing) {
        Serial.println("Got a Ping!");
    } else if(event == WebsocketsEvent::GotPong) {
        Serial.println("Got a Pong!");
    }
}

void Net::setUpWifi() {
  WiFi.begin(ssid, password);

  Serial.print("Connecting to WiFi ...");
  while (WiFi.status()!= WL_CONNECTED) {
    Serial.println(WiFi.status());
    delay(1000);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Connected to WiFi localIP: ");
  Serial.println(WiFi.localIP());
}

void Net::setUpWebsocket(CommandCallback callback) {
  // Serial.begin(9600);
  Serial.println("setup websocket");
  // client.onMessage(onMessageCallback2);

  client.onMessage([&](WebsocketsMessage msg){ onMessageCallback(msg); });
  client.onEvent([&](WebsocketsEvent event, String data) { onEventsCallback(event, data); });
  client.connect(websockets_url);
  commandCallback = callback;

  xTaskCreatePinnedToCore(&Net::wsDelegate, "WsTask", 4096, this, 1, NULL, 0);
}

WebsocketsClient& Net::ws() {
  return client;
}

void Net::loop() {
  client.poll();
}
