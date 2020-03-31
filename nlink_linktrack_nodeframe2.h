#ifndef NLINK_LINKTRACK_NODE_FRAME2_H
#define NLINK_LINKTRACK_NODE_FRAME2_H
#ifdef __cplusplus
extern "C" {
#endif
#include <stddef.h>
#include <stdint.h>

typedef struct {
  uint8_t role;
  uint8_t id;
  float dis;
  float fpRssi;
  float rxRssi;
} NLink_LinkTrack_Node2;

typedef struct {
  const size_t kFixedFrameLength;
  const uint8_t kFrameHeader;
  const uint8_t kFunctionMark;
  struct {
    uint8_t role;
    uint8_t id;
    uint32_t localTime;
    uint32_t systemTime;
    float voltage;
    float pos[3];
    float eop[3];
    float vel[3];
    float angle[3];
    float q[4];
    float imuGyro[3];
    float imuAcc[3];
    uint8_t validNodeCount;
    NLink_LinkTrack_Node2 *node[256];
  } data;

  uint8_t (*const unpackData)(const uint8_t *data, size_t dataLength);
} NLink_LinkTrack_NodeFrame2;

extern NLink_LinkTrack_NodeFrame2 nltNodeFrame2_;
#ifdef __cplusplus
}
#endif

#endif // NLINK_LINKTRACK_NODE_FRAME2_H
