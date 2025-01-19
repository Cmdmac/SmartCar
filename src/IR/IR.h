
#ifndef _H_IR_
#define _H_IR_
/*
 * Specify which protocol(s) should be used for decoding.
 * If no protocol is defined, all protocols (except Bang&Olufsen) are active.
 * This must be done before the #include <IRremote.hpp>
 */
#define DECODE_DENON        // Includes Sharp
#define DECODE_JVC
#define DECODE_KASEIKYO
#define DECODE_PANASONIC    // alias for DECODE_KASEIKYO
#define DECODE_LG
#define DECODE_NEC          // Includes Apple and Onkyo
#define DECODE_SAMSUNG
#define DECODE_SONY
#define DECODE_RC5
#define DECODE_RC6

#define DECODE_BOSEWAVE
#define DECODE_LEGO_PF
#define DECODE_MAGIQUEST
#define DECODE_WHYNTER
#define DECODE_FAST
//

#if !defined(RAW_BUFFER_LENGTH)
// For air condition remotes it requires 750. Default is 200.
#  if !((defined(RAMEND) && RAMEND <= 0x4FF) || (defined(RAMSIZE) && RAMSIZE < 0x4FF))
#define RAW_BUFFER_LENGTH  750
#  endif
#endif

//#define EXCLUDE_UNIVERSAL_PROTOCOLS // Saves up to 1000 bytes program memory.
//#define EXCLUDE_EXOTIC_PROTOCOLS // saves around 650 bytes program memory if all other protocols are active
//#define NO_LED_FEEDBACK_CODE      // saves 92 bytes program memory
//#define RECORD_GAP_MICROS 12000   // Default is 8000. Activate it for some LG air conditioner protocols
//#define SEND_PWM_BY_TIMER         // Disable carrier PWM generation in software and use (restricted) hardware PWM.
//#define USE_NO_SEND_PWM           // Use no carrier PWM, just simulate an active low receiver signal. Overrides SEND_PWM_BY_TIMER definition

// MARK_EXCESS_MICROS is subtracted from all marks and added to all spaces before decoding,
// to compensate for the signal forming of different IR receiver modules. See also IRremote.hpp line 142.
#define MARK_EXCESS_MICROS    20    // Adapt it to your IR receiver module. 20 is recommended for the cheap VS1838 modules.

//#define DEBUG // Activate this for lots of lovely debug output from the decoders.
// #include "PinDefinitionsAndMore.h"

// #include "IRremote.hpp"

// // Storage for the recorded code
// struct storedIRDataStruct {
//     IRData receivedIRData;
//     // extensions for sendRaw
//     uint8_t rawCode[RAW_BUFFER_LENGTH]; // The durations if raw
//     uint8_t rawCodeLength; // The length of the code
// } sStoredIRData;
// typedef std::function<void()> IRCb;


class IR
{
    private:
        /* data */
        static void startDelegate(void *pvParameters) {
            IR* instance = static_cast<IR*>(pvParameters);
            while(1) {
                instance->loop();
            }
            }

        void loop();

    public:

        void setup(int receivePin);
        void send() {
            // IrSender.sendRaw()
        }

    private:
        // IRCb mCallback;

};

#endif
