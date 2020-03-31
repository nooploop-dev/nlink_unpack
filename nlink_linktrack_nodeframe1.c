#include "nlink_linktrack_nodeframe1.h"
#include "nlink_utils.h"

NPACKED(
    typedef struct {
        uint8_t role;
        uint8_t id;
        NInt24 pos[3];
        uint8_t reserved[9];
    }) Node1_Raw;

NPACKED(
    typedef struct {
        uint8_t header[2];
        uint16_t frameLength;
        uint8_t role;
        uint8_t id;
        uint32_t systemTime;
        uint32_t localTime;
        uint8_t reserved0[10];
        uint16_t voltage;

        uint8_t validNodeCount;
        //nodes...
        //uint8_t checkSum;
    }) NLink_LinkTrack_Node_Frame1_Raw;

static NLink_LinkTrack_Node_Frame1_Raw frame_;

static uint8_t unpackData(const uint8_t *data, size_t dataLength) {
    assert(nltNodeFrame1_.kFixedFrameLength == sizeof (frame_));
    if(dataLength < nltNodeFrame1_.kFixedFrameLength || data[0] != nltNodeFrame1_.kFrameHeader || data[1] != nltNodeFrame1_.kFunctionMark) return 0;
    size_t frameLength = FRAME_LENGTH(data);
    if(dataLength < frameLength) return 0;
    if(!verifyCheckSum(data,frameLength)) return 0;


    static uint8_t initNeeded = 1;
    if(initNeeded){
        memset(nltNodeFrame1_.data.node,0,sizeof(nltNodeFrame1_.data.node));
        initNeeded = 0;
    }

    memcpy(&frame_, data, nltNodeFrame1_.kFixedFrameLength);

    nltNodeFrame1_.data.id =frame_.id;
    nltNodeFrame1_.data.localTime =frame_.localTime;
    nltNodeFrame1_.data.systemTime =frame_.systemTime;
    nltNodeFrame1_.data.voltage = frame_.voltage / kVoltageMultiply_;

    nltNodeFrame1_.data.validNodeCount = frame_.validNodeCount;
    Node1_Raw rawNode;
    for(size_t i=0;i<frame_.validNodeCount;++i){
        if(!nltNodeFrame1_.data.node[i]){
            nltNodeFrame1_.data.node[i] = malloc( sizeof(NLink_LinkTrack_Node1) );
        }

        memcpy(&rawNode,data + nltNodeFrame1_.kFixedFrameLength + i*sizeof(Node1_Raw),sizeof(Node1_Raw));

        NLink_LinkTrack_Node1 *node = nltNodeFrame1_.data.node[i];
        node->role = rawNode.role;
        node->id = rawNode.id;
        TRANSFORM_ARRAY_INT24(node->pos,rawNode.pos,kPosMultiply_)
    }
    return 1;
}

NLink_LinkTrack_NodeFrame1 nltNodeFrame1_ = {
    .kFixedFrameLength = 27,
    .kFrameHeader = 0x55,
    .kFunctionMark = 0x03,
    .unpackData = unpackData
};

