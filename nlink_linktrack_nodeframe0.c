#include "nlink_linktrack_nodeframe0.h"
#include "nlink_utils.h"
#include <stdio.h>

NPACKED(
    typedef struct {
        uint8_t header[2];
        uint16_t frameLength;
        uint8_t role;
        uint8_t id;
        uint8_t reserved[4];
        uint8_t validNodeCount;
        //nodes...
        //uint8_t checkSum;
    }) NLink_LinkTrack_Node_Frame0_Raw;

static NLink_LinkTrack_Node_Frame0_Raw frame_;

uint8_t unpackData(const uint8_t *data, size_t dataLength) {
    assert(nltNodeFrame0_.kFixedFrameLength == sizeof (frame_));
    if(dataLength < nltNodeFrame0_.kFixedFrameLength || data[0] != nltNodeFrame0_.kFrameHeader || data[1] != nltNodeFrame0_.kFunctionMark) return 0;
    size_t frameLength = FRAME_LENGTH(data);
    if(dataLength < frameLength) return 0;
    if(!verifyCheckSum(data,frameLength)) return 0;

    static uint8_t initNeeded = 1;
    if(initNeeded){
        memset(nltNodeFrame0_.data.node,0,sizeof(nltNodeFrame0_.data.node));
        initNeeded = 0;
    }

    memcpy(&frame_, data, nltNodeFrame0_.kFixedFrameLength);
    nltNodeFrame0_.data.role = frame_.role;
    nltNodeFrame0_.data.id = frame_.id;
    nltNodeFrame0_.data.validNodeCount = frame_.validNodeCount;

    for(size_t i=0,address = nltNodeFrame0_.kFixedFrameLength;i<frame_.validNodeCount;++i){
        const uint8_t *begin = data + address;
        size_t dataLength = (size_t)(begin[2] | begin[3] << 8);
        size_t currentNodeSize = 4 + dataLength;
        if(address + currentNodeSize > frameLength-1){
            printf("warning: address(%zu) + currentNodeSize(%zu) > frameLength(%zu)-1",address,currentNodeSize,frameLength);
            return 0;
        }

        if(!nltNodeFrame0_.data.node[i]){
            nltNodeFrame0_.data.node[i] = malloc( sizeof(NLink_LinkTrack_Node0) );
        }
        memcpy(nltNodeFrame0_.data.node[i],begin,currentNodeSize);

        address += currentNodeSize;
    }

    return 1;
}

NLink_LinkTrack_NodeFrame0 nltNodeFrame0_ = {
    .kFixedFrameLength = 11,
    .kFrameHeader = 0x55,
    .kFunctionMark = 0x02,
    .unpackData = unpackData
};

