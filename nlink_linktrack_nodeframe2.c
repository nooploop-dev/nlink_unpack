#include "nlink_linktrack_nodeframe2.h"
#include "nlink_utils.h"

NPACKED(
    typedef struct {
        uint8_t role;
        uint8_t id;
        NInt24 dis;
        uint8_t fpRssi;
        uint8_t rxRssi;
        uint32_t systemTime;
        uint8_t reserved[2];
    }) Node2_Raw;

NPACKED(
    typedef struct {
        uint8_t header[2];
        uint16_t frameLength;
        uint8_t role;
        uint8_t id;
        uint32_t systemTime;
        uint8_t eop[3];
        NInt24 pos[3];
        NInt24 vel[3];
        uint8_t reserved1[9];
        float imuGyro[3];
        float imuAcc[3];
        uint8_t reserved2[12];
        int16_t angle[3];
        float q[4];
        uint8_t reserved3[4];
        uint32_t localTime;
        uint8_t reserved4[10];
        uint16_t voltage;
        uint8_t validNodeCount;
        //nodes...
        //uint8_t checkSum;
    }) NLink_LinkTrack_Node_Frame2_Raw;

static NLink_LinkTrack_Node_Frame2_Raw frame_;

static uint8_t unpackData(const uint8_t *data, size_t dataLength) {
    assert(nltNodeFrame2_.kFixedFrameLength == sizeof (frame_));
    if(dataLength < nltNodeFrame2_.kFixedFrameLength || data[0] != nltNodeFrame2_.kFrameHeader || data[1] != nltNodeFrame2_.kFunctionMark) return 0;
    size_t frameLength = FRAME_LENGTH(data);
    if(dataLength < frameLength) return 0;
    if(!verifyCheckSum(data,frameLength)) return 0;

    static uint8_t initNeeded = 1;
    if(initNeeded){
        memset(nltNodeFrame2_.data.node,0,sizeof(nltNodeFrame2_.data.node));
        initNeeded = 0;
    }

    memcpy(&frame_, data, nltNodeFrame2_.kFixedFrameLength);
    nltNodeFrame2_.data.role =frame_.role;
    nltNodeFrame2_.data.id =frame_.id;
    nltNodeFrame2_.data.localTime = frame_.localTime;
    nltNodeFrame2_.data.systemTime =frame_.systemTime;
    nltNodeFrame2_.data.voltage = frame_.voltage / kVoltageMultiply_;
    TRANSFORM_ARRAY_INT24(nltNodeFrame2_.data.pos,frame_.pos,kPosMultiply_)
    TRANSFORM_ARRAY_INT24(nltNodeFrame2_.data.vel,frame_.vel,kVelMultiply_)
    TRANSFORM_ARRAY(nltNodeFrame2_.data.imuGyro,frame_.imuGyro,1)
    TRANSFORM_ARRAY(nltNodeFrame2_.data.imuAcc,frame_.imuAcc,1)
    TRANSFORM_ARRAY(nltNodeFrame2_.data.q,frame_.q,1)
    TRANSFORM_ARRAY(nltNodeFrame2_.data.angle,frame_.angle,kAngleMultiply_)
    TRANSFORM_ARRAY(nltNodeFrame2_.data.eop,frame_.eop,kEopMultiply_)

    nltNodeFrame2_.data.validNodeCount = frame_.validNodeCount;
    Node2_Raw rawNode;
    for(size_t i=0;i<frame_.validNodeCount;++i){
        if(!nltNodeFrame2_.data.node[i]){
            nltNodeFrame2_.data.node[i] = malloc( sizeof(NLink_LinkTrack_Node2) );
        }

        memcpy(&rawNode,data + nltNodeFrame2_.kFixedFrameLength + i*sizeof(Node2_Raw),sizeof(Node2_Raw));

        NLink_LinkTrack_Node2 *node = nltNodeFrame2_.data.node[i];
        node->role = rawNode.role;
        node->id = rawNode.id;
        node->dis = int24Value( rawNode.dis)/kDisMultiply_;
        node->fpRssi = rawNode.fpRssi / kRssiMultiply_;
        node->rxRssi = rawNode.rxRssi / kRssiMultiply_;
    }
    return 1;
}

NLink_LinkTrack_NodeFrame2 nltNodeFrame2_ = {
    .kFixedFrameLength = 119,
    .kFrameHeader = 0x55,
    .kFunctionMark = 0x04,
    .unpackData = unpackData
};
