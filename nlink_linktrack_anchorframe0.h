#ifndef NLINK_LINKTRACK_ANCHOR_FRAME0_H
#define NLINK_LINKTRACK_ANCHOR_FRAME0_H

#ifdef __cplusplus
extern "C" {
#endif
#include <stddef.h>
#include <stdint.h>

typedef struct {
  uint8_t id;
  uint8_t role;
  float pos[3];
  float dis[8];
} NLink_LinkTrack_Tag;

typedef struct {
  const size_t kFixedFrameLength;
  const uint8_t kFrameHeader;
  const uint8_t kFunctionMark;
  const uint8_t kTailCheck;
  struct {
    uint8_t role;
    uint8_t id;
    uint32_t localTime;
    uint32_t systemTime;
    float voltage;
    uint8_t validNodeCount;
    NLink_LinkTrack_Tag tag[30];
  } data;

  uint8_t (*const unpackData)(const uint8_t *data, size_t dataLength);
} NLink_LinkTrack_AnchorFrame0;

extern NLink_LinkTrack_AnchorFrame0 nltAnchorFrame0_;
#ifdef __cplusplus
}
#endif

#endif // NLINK_LINKTRACK_ANCHOR_FRAME0_H
