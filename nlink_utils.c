#include "nlink_utils.h"

const float kVoltageMultiply_ = 1000.0f;
const float kPosMultiply_ = 1000.0f;
const float kDisMultiply_ = 1000.0f;
const float kVelMultiply_ = 10000.0f;
const float kAngleMultiply_ = 100.0f;
const float kRssiMultiply_ = -2.0f;
const float kEopMultiply_ = 100.0f;

int32_t int24Value(NInt24 data) {
    uint8_t *byte = (uint8_t *)(&data);
    return (int32_t)(byte[0] << 8 | byte[1] << 16 | byte[2] << 24) / 256;
}

uint8_t verifyCheckSum(const uint8_t *data, size_t dataLength)
{
    uint8_t sum = 0;
    for(size_t i=0;i<dataLength-1;++i){
        sum += data[i];
    }
    return sum == data[dataLength-1];
}

size_t stringToHex(const char *str, uint8_t *out) {
    size_t outLength = 0;
    size_t cnt = 0;
    uint8_t high = 0, low = 0;
    uint8_t current = 0;
    uint8_t value = 0;
    uint8_t isHighValid = 0;
    while ((current = str[cnt])) {
        ++cnt;
        if (current >= '0' && current <= '9') {
            value = (uint8_t) (current - '0');
        } else if (current >= 'a' && current <= 'f') {
            value = (uint8_t) (current - 'a' + 10);
        } else if (current >= 'A' && current <= 'F') {
            value = (uint8_t) (current - 'A' + 10);
        } else {
            continue;
        }
        if (!isHighValid) {
            high = value;
            isHighValid = 1;
        } else {
            low = value;
            out[outLength] = (uint8_t) (high << 4 | low);
            ++outLength;
            isHighValid = 0;
        }
    }
    return outLength;
}

void updateCheckSum(uint8_t *data, size_t dataLength)
{
    uint8_t sum = 0;
    for(size_t i=0;i<dataLength-1;++i){
        sum += data[i];
    }
    data[dataLength-1] = sum;
}
