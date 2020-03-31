#ifndef NLINK_LINKTRACK_TAG_FRAME0_H
#define NLINK_LINKTRACK_TAG_FRAME0_H
#ifdef __cplusplus
extern "C" {
#endif
#include <stddef.h>
#include <stdint.h>

typedef struct {
  const size_t kFixedFrameLength;
  const uint8_t kFrameHeader;
  const uint8_t kFunctionMark;
  struct {
    uint8_t role;
    uint8_t id;
    float pos[3];
    float eop[3];
    float vel[3];
    float dis[8];
    float imuGyro[3];
    float imuAcc[3];
    float angle[3];
    float q[4];
    uint32_t localTime;
    uint32_t systemTime;
    float voltage;
  } data;

  uint8_t (*const unpackData)(const uint8_t *data, size_t dataLength);
} NLink_LinkTrack_TagFrame0;

extern NLink_LinkTrack_TagFrame0 nltTagFrame0_;

#ifdef __cplusplus
}
#endif
#endif // NLINK_LINKTRACK_TAG_FRAME0_H
