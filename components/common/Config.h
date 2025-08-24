#pragma once

// WIFI 
#define WIFI_SSID           "Stark"  
#define WIFI_PASSWORD       "fengzhiping,1101"  


#define STARK_BOARD

#if defined(STARK_BOARD)
#include "StarkBoard.h"
#elif defined(LICHUANG_S3)
#include "LichuangS3.h"
#endif