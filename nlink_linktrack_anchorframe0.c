#include "nlink_linktrack_anchorframe0.h"
#include "nlink_utils.h"

NPACKED(
    typedef struct {
        uint8_t id;
        uint8_t role;
        NInt24 pos[3];
        uint16_t dis[8];
    }) TagRaw;

NPACKED(
    typedef struct {
        uint8_t header[2];
        TagRaw tag[30];
        uint8_t reserved0[67];
        uint32_t localTime;
        uint8_t reserved1[4];
        uint16_t voltage;
        uint32_t systemTime;
        uint8_t id;
        uint8_t role;
        uint8_t fixedCheck;
    }) NLink_LinkTrack_AnchorFrame0_Raw;

static NLink_LinkTrack_AnchorFrame0_Raw frame_;

static uint8_t unpackData(const uint8_t *data, size_t dataLength) {
    assert(nltAnchorFrame0_.kFixedFrameLength == sizeof (frame_));
    if(dataLength < nltAnchorFrame0_.kFixedFrameLength || data[0] != nltAnchorFrame0_.kFrameHeader || data[1] != nltAnchorFrame0_.kFunctionMark || data[nltAnchorFrame0_.kFixedFrameLength-1] != nltAnchorFrame0_.kTailCheck) return 0;

    memcpy(&frame_, data, nltAnchorFrame0_.kFixedFrameLength);
    nltAnchorFrame0_.data.role =frame_.role;
    nltAnchorFrame0_.data.id =frame_.id;
    nltAnchorFrame0_.data.localTime =frame_.localTime;
    nltAnchorFrame0_.data.systemTime =frame_.systemTime;
    nltAnchorFrame0_.data.voltage = frame_.voltage / kVoltageMultiply_;

    nltAnchorFrame0_.data.validNodeCount = 0;
    for(size_t i=0,count=ARRAY_LENGTH(nltAnchorFrame0_.data.tag);i<count;++i){
        if(frame_.tag[i].id == 0xff) continue;

        uint8_t index = nltAnchorFrame0_.data.validNodeCount;
        nltAnchorFrame0_.data.tag[index].role = frame_.tag[i].role;
        nltAnchorFrame0_.data.tag[index].id = frame_.tag[i].id;
        TRANSFORM_ARRAY_INT24(nltAnchorFrame0_.data.tag[index].pos,frame_.tag[i].pos,kPosMultiply_)
        TRANSFORM_ARRAY(nltAnchorFrame0_.data.tag[index].dis,frame_.tag[i].dis,100.0f)

        ++nltAnchorFrame0_.data.validNodeCount;
    }
    return 1;
}

NLink_LinkTrack_AnchorFrame0 nltAnchorFrame0_ = {
    .kFixedFrameLength = 896,
    .kFrameHeader = 0x55,
    .kFunctionMark = 0x00,
    .kTailCheck = 0xee,
    .unpackData = unpackData
};

