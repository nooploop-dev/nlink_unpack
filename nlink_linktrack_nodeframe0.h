#ifndef NLINK_LINKTRACK_NODE_FRAME0_H
#define NLINK_LINKTRACK_NODE_FRAME0_H
#ifdef __cplusplus
extern "C" {
#endif
#include <stddef.h>
#include <stdint.h>

typedef struct {
  uint8_t role;
  uint8_t id;
  uint16_t dataLength;
  //可根据自身情况设置单节点单次最大数传长度，上限参考
  // LinkTrack Datasheet
  uint8_t data[224];
} NLink_LinkTrack_Node0;

typedef struct {
  const size_t kFixedFrameLength;
  const uint8_t kFrameHeader;
  const uint8_t kFunctionMark;
  struct {
    uint8_t role;
    uint8_t id;
    uint8_t validNodeCount;
    NLink_LinkTrack_Node0 *node[256];
  } data;

  uint8_t (*const unpackData)(const uint8_t *data, size_t dataLength);
} NLink_LinkTrack_NodeFrame0;

extern NLink_LinkTrack_NodeFrame0 nltNodeFrame0_;
#ifdef __cplusplus
}
#endif

#endif // NLINK_LINKTRACK_NODE_FRAME0_H
