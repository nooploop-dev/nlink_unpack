#include "nlink_tofsense_frame0.h"
#include "nlink_utils.h"

NPACKED(
    typedef struct {
        uint8_t header[2];
        uint8_t reserved0;
        uint8_t id;
        uint32_t systemTime;
        NInt24 distance;
        uint8_t distanceStatus;
        uint16_t signalStrength;
        uint8_t reserved1;
        uint8_t checkSum;
    }) NLink_TOFSense_Frame0_Raw;

static NLink_TOFSense_Frame0_Raw frame_;

static uint8_t unpackData(const uint8_t *data, size_t dataLength) {
    assert(ntsFrame0_.kFixedFrameLength == 16);
    if(dataLength < ntsFrame0_.kFixedFrameLength || data[0] != ntsFrame0_.kFrameHeader || data[1] != ntsFrame0_.kFunctionMark) return 0;
    if(!verifyCheckSum(data,ntsFrame0_.kFixedFrameLength)) return 0;

    memcpy(&frame_, data, ntsFrame0_.kFixedFrameLength);
    ntsFrame0_.data.id =frame_.id;
    ntsFrame0_.data.systemTime =frame_.systemTime;
    ntsFrame0_.data.distanceStatus =frame_.distanceStatus;
    ntsFrame0_.data.signalStrength =frame_.signalStrength;
    ntsFrame0_.data.distance =int24Value(frame_.distance)/1000.0f;

    return 1;
}

NLink_TOFSense_Frame0 ntsFrame0_ = {
    .kFixedFrameLength = 16,
    .kFrameHeader = 0x57,
    .kFunctionMark = 0x00,
    .unpackData = unpackData
};
