#ifndef NLINK_UTILS_H
#define NLINK_UTILS_H
#ifdef __cplusplus
extern "C" {
#endif
#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// Macro to define packed structures
#ifdef __GNUC__
#define NPACKED(__Declaration__) __Declaration__ __attribute__((packed))
#else
#define NPACKED(__Declaration__)                                               \
  __pragma(pack(push, 1)) __Declaration__ __pragma(pack(pop))
#endif

#define ARRAY_LENGTH(X) (sizeof(X) / sizeof(X[0]))

#define FRAME_LENGTH(X) ((size_t)(X[2] | X[3] << 8))

extern const float kVoltageMultiply_;
extern const float kPosMultiply_;
extern const float kDisMultiply_;
extern const float kVelMultiply_;
extern const float kAngleMultiply_;
extern const float kRssiMultiply_;
extern const float kEopMultiply_;

typedef struct { uint8_t byteArray[3]; } NInt24;
int32_t int24Value(NInt24 data);

#define TRANSFORM_ARRAY(DEST, SRC, MULTIPLY)                                   \
  for (size_t _CNT = 0; _CNT < sizeof(SRC) / sizeof(SRC[0]); ++_CNT) {         \
    DEST[_CNT] = SRC[_CNT] / MULTIPLY;                                         \
  }

#define TRANSFORM_ARRAY_INT24(DEST, SRC, MULTIPLY)                             \
  for (size_t _CNT = 0; _CNT < sizeof(SRC) / sizeof(SRC[0]); ++_CNT) {         \
    DEST[_CNT] = int24Value(SRC[_CNT]) / MULTIPLY;                             \
  }

uint8_t verifyCheckSum(const uint8_t *data, size_t dataLength);
void updateCheckSum(uint8_t *data, size_t dataLength);
size_t stringToHex(const char *str, uint8_t *out);
#ifdef __cplusplus
}
#endif
#endif // NLINK_UTILS_H
