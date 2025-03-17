idf_component_register(SRCS "upload_voice.c" "main.cpp"
                    INCLUDE_DIRS "."
                    REQUIRES arduino-esp32 ArduinoWebsockets arduinojson common ibeacon compass led servo net ir gyroscope ioextend camera tft sr max1704x)
