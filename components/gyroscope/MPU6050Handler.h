#ifndef _MPU6050_HANDLER_H
#define _MPU6050_HANDLER_H

#include <Wire.h>
#include <I2Cdev.h>
#include <MPU6050.h>
#define OUTPUT_READABLE_ACCELGYRO
//#define OUTPUT_BINARY_ACCELGYRO
class MPU6050Handler {

    private:
        int16_t ax, ay, az;
        int16_t gx, gy, gz;
        MPU6050 mpu;

    public:
        void setup() {
            Wire.begin(9, 10);
            /*Initialize device and check connection*/ 
            Serial.println("Initializing MPU...");
            mpu.initialize();
            Serial.println("Testing MPU6050 connection...");
            if(mpu.testConnection() ==  false){ 
                Serial.println("MPU6050 connection failed");
                while(true);
            }
            else{
                Serial.println("MPU6050 connection successful");
            }

            /* Use the code below to change accel/gyro offset values. Use MPU6050_Zero to obtain the recommended offsets */ 
            Serial.println("Updating internal sensor offsets...\n");
            mpu.setXAccelOffset(0); //Set your accelerometer offset for axis X
            mpu.setYAccelOffset(0); //Set your accelerometer offset for axis Y
            mpu.setZAccelOffset(0); //Set your accelerometer offset for axis Z
            mpu.setXGyroOffset(0);  //Set your gyro offset for axis X
            mpu.setYGyroOffset(0);  //Set your gyro offset for axis Y
            mpu.setZGyroOffset(0);  //Set your gyro offset for axis Z
            /*Print the defined offsets*/
            Serial.print("\t");
            Serial.print(mpu.getXAccelOffset());
            Serial.print("\t");
            Serial.print(mpu.getYAccelOffset()); 
            Serial.print("\t");
            Serial.print(mpu.getZAccelOffset());
            Serial.print("\t");
            Serial.print(mpu.getXGyroOffset()); 
            Serial.print("\t");
            Serial.print(mpu.getYGyroOffset());
            Serial.print("\t");
            Serial.print(mpu.getZGyroOffset());
            Serial.print("\n");
        }

        void loop() {
            /* Read raw accel/gyro data from the module. Other methods commented*/
            mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
            //mpu.getAcceleration(&ax, &ay, &az);
            //mpu.getRotation(&gx, &gy, &gz);

            /*Print the obtained data on the defined format*/
            #ifdef OUTPUT_READABLE_ACCELGYRO
                Serial.print("a/g:\t");
                Serial.print(ax); Serial.print("\t");
                Serial.print(ay); Serial.print("\t");
                Serial.print(az); Serial.print("\t");
                Serial.print(gx); Serial.print("\t");
                Serial.print(gy); Serial.print("\t");
                Serial.println(gz);
            #endif

            #ifdef OUTPUT_BINARY_ACCELGYRO
                Serial.write((uint8_t)(ax >> 8)); Serial.write((uint8_t)(ax & 0xFF));
                Serial.write((uint8_t)(ay >> 8)); Serial.write((uint8_t)(ay & 0xFF));
                Serial.write((uint8_t)(az >> 8)); Serial.write((uint8_t)(az & 0xFF));
                Serial.write((uint8_t)(gx >> 8)); Serial.write((uint8_t)(gx & 0xFF));
                Serial.write((uint8_t)(gy >> 8)); Serial.write((uint8_t)(gy & 0xFF));
                Serial.write((uint8_t)(gz >> 8)); Serial.write((uint8_t)(gz & 0xFF));
            #endif
            delay(100);
        }
};
#endif