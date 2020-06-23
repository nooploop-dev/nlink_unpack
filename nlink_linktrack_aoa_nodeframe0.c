#include "nlink_linktrack_aoa_nodeframe0.h"
#include "nlink_utils.h"

NPACKED(
    typedef struct {
        uint8_t role;
        uint8_t id;
        NInt24 dis;
        int16_t angle;
        uint8_t fpRssi;
        uint8_t rxRssi;
        uint8_t reserved[2];
    })
Aoa_Node0_Raw;

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
    })
NLink_LinkTrack_Aoa_Node_Frame0_Raw;

static NLink_LinkTrack_Aoa_Node_Frame0_Raw frame_;

static uint8_t unpackData(const uint8_t *data, size_t dataLength)
{
    assert(nltAoaNodeFrame0_.kFixedFrameLength == sizeof(frame_));
    if (dataLength < nltAoaNodeFrame0_.kFixedFrameLength || data[0] != nltAoaNodeFrame0_.kFrameHeader || data[1] != nltAoaNodeFrame0_.kFunctionMark)
        return 0;
    size_t frameLength = FRAME_LENGTH(data);
    if (dataLength < frameLength)
        return 0;
    if (!verifyCheckSum(data, frameLength))
        return 0;

    static uint8_t initNeeded = 1;
    if (initNeeded)
    {
        memset(nltAoaNodeFrame0_.data.node, 0, sizeof(nltAoaNodeFrame0_.data.node));
        initNeeded = 0;
    }

    memcpy(&frame_, data, nltAoaNodeFrame0_.kFixedFrameLength);
    nltAoaNodeFrame0_.data.role = frame_.role;
    nltAoaNodeFrame0_.data.id = frame_.id;
    nltAoaNodeFrame0_.data.localTime = frame_.localTime;
    nltAoaNodeFrame0_.data.systemTime = frame_.systemTime;
    nltAoaNodeFrame0_.data.voltage = frame_.voltage / kVoltageMultiply_;

    nltAoaNodeFrame0_.data.validNodeCount = frame_.validNodeCount;
    Aoa_Node0_Raw rawNode;
    for (size_t i = 0; i < frame_.validNodeCount; ++i)
    {
        if (!nltAoaNodeFrame0_.data.node[i])
        {
            nltAoaNodeFrame0_.data.node[i] = malloc(sizeof(NLink_LinkTrack_Aoa_Node0));
        }

        memcpy(&rawNode, data + nltAoaNodeFrame0_.kFixedFrameLength + i * sizeof(Aoa_Node0_Raw), sizeof(Aoa_Node0_Raw));

        NLink_LinkTrack_Aoa_Node0 *node = nltAoaNodeFrame0_.data.node[i];
        node->role = rawNode.role;
        node->id = rawNode.id;
        node->dis = int24Value(rawNode.dis) / kDisMultiply_;
        node->angle = rawNode.angle / kAngleMultiply_;
        node->fpRssi = rawNode.fpRssi / kRssiMultiply_;
        node->rxRssi = rawNode.rxRssi / kRssiMultiply_;
    }
    return 1;
}

NLink_LinkTrack_Aoa_NodeFrame0 nltAoaNodeFrame0_ = {
    .kFixedFrameLength = 21,
    .kFrameHeader = 0x55,
    .kFunctionMark = 0x07,
    .unpackData = unpackData};
