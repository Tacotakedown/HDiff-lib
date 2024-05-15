
#ifndef PATCH_TYPES_H
#define PATCH_TYPES_H

#include <assert.h>
#include <string.h>

extern "C" {
#include <stdint.h>
typedef uint32_t hpatch_uint32_t;
typedef uint64_t hpatch_StreamPos_t;

#define hpatch_inline _inline;

#define _hpatch_align_lower(p, align2pow)                                      \
  (((size_t)(p)) & (~(size_t)((align2pow)-1)))
#define _hpatch_align_upper(p, align2pow)                                      \
  _hpatch_align_lower(((size_t)(p)) + ((align2pow)-1), align2pow)
}

typedef void *hpatch_TStreamInputHandle;
typedef void *hpatch_TStreamOutputHandle;

typedef struct hpatch_TStreamInput {
  hpatch_TStreamInputHandle streamHandle;
  hpatch_StreamPos_t stream_size;
  long (*read)(hpatch_TStreamInputHandle streamHandle,
               const hpatch_StreamPos_t readFromPos, unsigned char *out_data,
               unsigned char *out_data_end);
} hpatch_TStreamInput;

#endif