#include "IR.h"



IR::IR() : mIRRecv(IR_RX_GPIO_NUM, IR_BUFFER_SIZE), mTaskHandle(NULL) {
  // irrecv.setTolerance(kTolerancePercentage);  // Override the default tolerance.

}

void IR::startLearn() {
  mIRRecv.enableIRIn();  // Start the receiver
  xTaskCreatePinnedToCore(&IR::delegate, "IRReadTask", 4096 * 2, this, 1, &mTaskHandle, 0);
}
    
void IR::stopLearn() {
  if (mTaskHandle != NULL) {
    vTaskDelete(&mTaskHandle);
  }
}

void IR::send() {

}

void IR::loop() {
  // Check if the IR code has been received.
  if (mIRRecv.decode(&mDecodeResults)) {
    // Display a crude timestamp.
    uint32_t now = millis();
    Serial.printf(D_STR_TIMESTAMP " : %06u.%03u\n", now / 1000, now % 1000);
    // Check if we got an IR message that was to big for our capture buffer.
    if (mDecodeResults.overflow)
      Serial.printf("mDecodeResults overflow");
      // Serial.printf(D_WARN_BUFFERFULL "\n", kCaptureBufferSize);
    // Display the library version the message was captured with.
    Serial.println(D_STR_LIBRARY "   : v" _IRREMOTEESP8266_VERSION_STR "\n");
    // Display the tolerance percentage if it has been change from the default.
    // if (kTolerancePercentage != kTolerance)
    //   Serial.printf(D_STR_TOLERANCE " : %d%%\n", kTolerancePercentage);
    // Display the basic output of what we found.
    Serial.print(resultToHumanReadableBasic(&mDecodeResults));
    // Display any extra A/C info if we have it.
    String description = IRAcUtils::resultAcToString(&mDecodeResults);
    if (description.length()) Serial.println(D_STR_MESGDESC ": " + description);
    yield();  // Feed the WDT as the text output can take a while to print.
// #if LEGACY_TIMING_INFO
//     // Output legacy RAW timing info of the result.
//     Serial.println(resultToTimingInfo(&results));
//     yield();  // Feed the WDT (again)
// #endif  // LEGACY_TIMING_INFO
    // Output the results as source code
    Serial.println(resultToSourceCode(&mDecodeResults));
    Serial.println();    // Blank line between entries
    yield();             // Feed the WDT (again)
  }
}