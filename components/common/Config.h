#pragma once

#define LICHUANG_S3

#if defined(STARK_BOARD)
#include "StarkBoard.h"
#elif defined(LICHUANG_S3)
#include "LichuangS3.h"
#endif