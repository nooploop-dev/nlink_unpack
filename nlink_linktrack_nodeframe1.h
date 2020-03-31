#ifndef NLINK_LINKTRACK_NODE_FRAME1_H
#define NLINK_LINKTRACK_NODE_FRAME1_H
#ifdef __cplusplus
extern "C" {
#endif
#include <stddef.h>
#include <stdint.h>

typedef struct {
  uint8_t role;
  uint8_t id;
  float pos[3];
} NLink_LinkTrack_Node1;

typedef struct {
  const size_t kFixedFrameLength;
  const uint8_t kFrameHeader;
  const uint8_t kFunctionMark;
  struct {
    uint8_t role;
    uint8_t id;
    uint32_t systemTime;
    uint32_t localTime;
    float voltage;
    uint8_t validNodeCount;
    NLink_LinkTrack_Node1 *node[256];
  } data;

  uint8_t (*const unpackData)(const uint8_t *data, size_t dataLength);
} NLink_LinkTrack_NodeFrame1;

extern NLink_LinkTrack_NodeFrame1 nltNodeFrame1_;
#ifdef __cplusplus
}
#endif
#endif // NLINK_LINKTRACK_NODE_FRAME1_H
