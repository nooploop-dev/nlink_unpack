#ifndef NLINK_TOFSENSE_FRAME0_H
#define NLINK_TOFSENSE_FRAME0_H
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
    uint8_t id;
    uint32_t systemTime;
    float distance;
    uint8_t distanceStatus;
    uint16_t signalStrength;
  } data;

  uint8_t (*const unpackData)(const uint8_t *data, size_t dataLength);
} NLink_TOFSense_Frame0;

extern NLink_TOFSense_Frame0 ntsFrame0_;

#ifdef __cplusplus
}
#endif
#endif // NLINK_TOFSENSE_FRAME0_H
