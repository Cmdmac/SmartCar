idf_component_register(SRCS "DataChannel.cpp" "Application.cpp" "main.cpp"
                    INCLUDE_DIRS "."
                    REQUIRES arduino-esp32 ArduinoWebsockets arduinojson common ibeacon compass led servo net ir gyroscope ioextend camera tft sr power)
