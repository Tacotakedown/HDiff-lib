#ifndef HPatch_checksum_plugin_h
#define HPatch_checksum_plugin_h
#include "patch_types.h"
#ifdef __cplusplus
extern "C" {
#endif

typedef void *hpatch_checksumHandle;
typedef struct hpatch_TChecksum {
  // return type tag; strlen(result)<=hpatch_kMaxPluginTypeLength; (Note:result
  // lifetime)
  const char *(*checksumType)(void);       // ascii cstring,cannot contain '&'
  hpatch_size_t (*checksumByteSize)(void); // result<=hpatch_kStreamCacheSize
  hpatch_checksumHandle (*open)(struct hpatch_TChecksum *plugin);
  void (*close)(struct hpatch_TChecksum *plugin, hpatch_checksumHandle handle);
  void (*begin)(hpatch_checksumHandle handle);
  void (*append)(hpatch_checksumHandle handle, const unsigned char *part_data,
                 const unsigned char *part_data_end);
  void (*end)(hpatch_checksumHandle handle, unsigned char *checksum,
              unsigned char *checksum_end);
} hpatch_TChecksum;

#ifdef __cplusplus
}
#endif
#endif