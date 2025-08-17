idf_component_register(SRCS "DataChannel.cpp" "Application.cpp" "main.cpp"
                    INCLUDE_DIRS "."
                    REQUIRES ArduinoWebsockets arduinojson common ibeacon compass led servo net ir gyroscope ioextend camera tft power speaker mic
                    )
