#pragma once

#define HEADER_LEN      (128)
#define MAGIC_NUM       { 0x4F, 0x52, 0x49, 0x4E, 0x43, 0x48, 0x41, 0x4E }
#define MAGIC_NUM_LEN   (8)

#include "enums.cpp"


void safe_free(void *value) {
    free(value);
    value = nullptr;
}
