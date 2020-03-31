#include "nlink_linktrack_tagframe0.h"
#include "nlink_utils.h"

NPACKED(
    typedef struct {
        uint8_t header[2];
        uint8_t id;
        uint8_t role;
        NInt24 pos[3];
        NInt24 vel[3];
        NInt24 dis[8];
        float imuGyro[3];
        float imuAcc[3];
        uint8_t reserved1[12];
        int16_t angle[3];
        float q[4];
        uint8_t reserved2[4];
        uint32_t localTime;
        uint32_t systemTime;
        uint8_t reserved3[1];
        uint8_t eop[3];
        uint16_t voltage;
        uint8_t reserved4[5];
        uint8_t checkSum;
    }) NLink_LinkTrack_TagFrame0_Raw;

static NLink_LinkTrack_TagFrame0_Raw frame_;

static uint8_t unpackData(const uint8_t *data,size_t dataLength) {
    assert(nltTagFrame0_.kFixedFrameLength == sizeof (frame_));
    if(dataLength < nltTagFrame0_.kFixedFrameLength || data[0] != nltTagFrame0_.kFrameHeader || data[1] != nltTagFrame0_.kFunctionMark) return 0;
    if(!verifyCheckSum(data,nltTagFrame0_.kFixedFrameLength)) return 0;

    memcpy(&frame_, data, nltTagFrame0_.kFixedFrameLength);
    nltTagFrame0_.data.role =frame_.role;
    nltTagFrame0_.data.id =frame_.id;
    nltTagFrame0_.data.localTime =frame_.localTime;
    nltTagFrame0_.data.systemTime =frame_.systemTime;
    nltTagFrame0_.data.voltage = frame_.voltage / kVoltageMultiply_;

    TRANSFORM_ARRAY_INT24(nltTagFrame0_.data.pos,frame_.pos,kPosMultiply_)
    TRANSFORM_ARRAY_INT24(nltTagFrame0_.data.vel,frame_.vel,kVelMultiply_)
    TRANSFORM_ARRAY_INT24(nltTagFrame0_.data.dis,frame_.dis,kDisMultiply_)
    TRANSFORM_ARRAY(nltTagFrame0_.data.imuGyro,frame_.imuGyro,1)
    TRANSFORM_ARRAY(nltTagFrame0_.data.imuAcc,frame_.imuAcc,1)
    TRANSFORM_ARRAY(nltTagFrame0_.data.q,frame_.q,1)
    TRANSFORM_ARRAY(nltTagFrame0_.data.angle,frame_.angle,kAngleMultiply_)
    TRANSFORM_ARRAY(nltTagFrame0_.data.eop,frame_.eop,kEopMultiply_)

    return 1;
}

NLink_LinkTrack_TagFrame0 nltTagFrame0_ = {
    .kFixedFrameLength = 128,
    .kFrameHeader = 0x55,
    .kFunctionMark = 0x01,
    .unpackData = unpackData
};


