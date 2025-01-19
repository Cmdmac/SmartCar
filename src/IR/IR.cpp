#include "IR.h"

// #define IR_RECEIVE_PIN 8
void IR::setup(int receivePin) {
    // this->mCallback = callback;
     // Start the receiver and if not 3. parameter specified, take LED_BUILTIN pin from the internal boards definition as default feedback LED
    // IrReceiver.begin(receivePin, ENABLE_LED_FEEDBACK);
    // xTaskCreatePinnedToCore(&IR::startDelegate, "IR task", 4096, this, 1, NULL, 0);
}

void IR::loop() {
    /*
     * Check if received data is available and if yes, try to decode it.
     * Decoded result is in the IrReceiver.decodedIRData structure.
     *
     * E.g. command is in IrReceiver.decodedIRData.command
     * address is in command is in IrReceiver.decodedIRData.address
     * and up to 32 bit raw data in IrReceiver.decodedIRData.decodedRawData
     */
    // if (IrReceiver.decode()) {

    //     Serial.print(F(", decoded address: "));
    //     Serial.print(IrReceiver.decodedIRData.address, HEX);
    //     Serial.print(F(", decoded command: "));
    //     Serial.println(IrReceiver.decodedIRData.command, HEX);
    //     /*
    //      * Print a summary of received data
    //      */
    //     if (IrReceiver.decodedIRData.protocol == UNKNOWN) {
    //         Serial.println(F("Received noise or an unknown (or not yet enabled) protocol"));
    //         // We have an unknown protocol here, print extended info
    //         // IrReceiver.printIRResultRawFormatted(&Serial, true);
    //         IrReceiver.resume(); // Do it here, to preserve raw data for printing with printIRResultRawFormatted()
    //     } else {
    //         // mCallback();
    //         IrReceiver.resume(); // Early enable receiving of the next IR frame
    //         // IrReceiver.printIRResultShort(&Serial);
    //         // IrReceiver.printIRSendUsage(&Serial);
    //     }
    //     Serial.println();

    //     /*
    //      * Finally, check the received data and perform actions according to the received command
    //      */
    //     if (IrReceiver.decodedIRData.command == 0x10) {
    //         // do something
    //     } else if (IrReceiver.decodedIRData.command == 0x11) {
    //         // do something else
    //     }
    // }
}