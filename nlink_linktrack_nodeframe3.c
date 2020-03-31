#include "nlink_linktrack_nodeframe3.h"
#include "nlink_utils.h"

NPACKED(
    typedef struct {
        uint8_t role;
        uint8_t id;
        NInt24 dis;
        uint8_t fpRssi;
        uint8_t rxRssi;
    }) Node3_Raw;

NPACKED(
    typedef struct {
        uint8_t header[2];
        uint16_t frameLength;
        uint8_t role;
        uint8_t id;
        uint32_t localTime;
        uint32_t systemTime;
        uint8_t reserved0[4];
        uint16_t voltage;
        uint8_t validNodeCount;
        //nodes...
        //uint8_t checkSum;
    }) NLink_LinkTrack_Node_Frame3_Raw;

static NLink_LinkTrack_Node_Frame3_Raw frame_;

static uint8_t unpackData(const uint8_t *data, size_t dataLength) {
    assert(nltNodeFrame3_.kFixedFrameLength == sizeof (frame_));
    if(dataLength < nltNodeFrame3_.kFixedFrameLength || data[0] != nltNodeFrame3_.kFrameHeader || data[1] != nltNodeFrame3_.kFunctionMark) return 0;
    size_t frameLength = FRAME_LENGTH(data);
    if(dataLength < frameLength) return 0;
    if(!verifyCheckSum(data,frameLength)) return 0;

    static uint8_t initNeeded = 1;
    if(initNeeded){
        memset(nltNodeFrame3_.data.node,0,sizeof(nltNodeFrame3_.data.node));
        initNeeded = 0;
    }

    memcpy(&frame_, data, nltNodeFrame3_.kFixedFrameLength);
    nltNodeFrame3_.data.role =frame_.role;
    nltNodeFrame3_.data.id =frame_.id;
    nltNodeFrame3_.data.localTime = frame_.localTime;
    nltNodeFrame3_.data.systemTime =frame_.systemTime;
    nltNodeFrame3_.data.voltage = frame_.voltage / kVoltageMultiply_;

    nltNodeFrame3_.data.validNodeCount = frame_.validNodeCount;
    Node3_Raw rawNode;
    for(size_t i=0;i<frame_.validNodeCount;++i){
        if(!nltNodeFrame3_.data.node[i]){
            nltNodeFrame3_.data.node[i] = malloc( sizeof(NLink_LinkTrack_Node3) );
        }

        memcpy(&rawNode,data + nltNodeFrame3_.kFixedFrameLength + i*sizeof(Node3_Raw),sizeof(Node3_Raw));

        NLink_LinkTrack_Node3 *node = nltNodeFrame3_.data.node[i];
        node->role = rawNode.role;
        node->id = rawNode.id;
        node->dis = int24Value( rawNode.dis)/kDisMultiply_;
        node->fpRssi = rawNode.fpRssi / kRssiMultiply_;
        node->rxRssi = rawNode.rxRssi / kRssiMultiply_;
    }
    return 1;
}

NLink_LinkTrack_NodeFrame3 nltNodeFrame3_ = {
    .kFixedFrameLength = 21,
    .kFrameHeader = 0x55,
    .kFunctionMark = 0x05,
    .unpackData = unpackData
};
