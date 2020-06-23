#ifndef NLINK_LINKTRACK_AOA_NODE_FRAME0_H
#define NLINK_LINKTRACK_AOA_NODE_FRAME0_H
#ifdef __cplusplus
extern "C"
{
#endif
#include <stddef.h>
#include <stdint.h>

  typedef struct
  {
    uint8_t role;
    uint8_t id;
    float dis;
    float angle;
    float fpRssi;
    float rxRssi;
  } NLink_LinkTrack_Aoa_Node0;

  typedef struct
  {
    const size_t kFixedFrameLength;
    const uint8_t kFrameHeader;
    const uint8_t kFunctionMark;
    struct
    {
      uint8_t role;
      uint8_t id;
      uint32_t localTime;
      uint32_t systemTime;
      float voltage;
      uint8_t validNodeCount;
      NLink_LinkTrack_Aoa_Node0 *node[256];
    } data;

    uint8_t (*const unpackData)(const uint8_t *data, size_t dataLength);
  } NLink_LinkTrack_Aoa_NodeFrame0;

  extern NLink_LinkTrack_Aoa_NodeFrame0 nltAoaNodeFrame0_;
#ifdef __cplusplus
}
#endif

#endif  // NLINK_LINKTRACK_AOA_NODE_FRAME0_H
