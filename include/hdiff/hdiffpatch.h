#ifndef HDiffPatch_H
#define HDiffPatch_H

#include "diff_types.h"
#include "patch_types.h"
#include <vector>

static const int kMinSingleMatchScore_default = 6;
static const size_t kMatchBlockSize_default = (1 << 6);
static const size_t kMatchBlockSize_min = 4;
static const size_t kDefaultPatchStepMemSize = 1024 * 256;

void create_diff(const unsigned char *newData, const unsigned char *newData_end,
                 const unsigned char *oldData, const unsigned char *oldData_end,
                 std::vector<unsigned char> &out_diff,
                 int kMinSingleMatchScore = kMinSingleMatchScore_default,
                 bool isUseBigCacheMatch = false, size_t threadNum = 1);

bool check_diff(const unsigned char *newData, const unsigned char *newData_end,
                const unsigned char *oldData, const unsigned char *oldData_end,
                const unsigned char *diff, const unsigned char *diff_end);
bool check_diff(const hpatch_TStreamInput *newData,
                const hpatch_TStreamInput *oldData,
                const hpatch_TStreamInput *diff);

void create_compressed_diff(
    const unsigned char *newData, const unsigned char *newData_end,
    const unsigned char *oldData, const unsigned char *oldData_end,
    std::vector<unsigned char> &out_diff,
    const hdiff_TCompress *compressPlugin = 0,
    int kMinSingleMatchScore = kMinSingleMatchScore_default,
    bool isUseBigCacheMatch = false, ICoverLinesListener *listener = 0,
    size_t threadNum = 1);

void create_compressed_diff_stream(
    const hpatch_TStreamInput *newData, const hpatch_TStreamInput *oldData,
    const hpatch_TStreamOutput *out_diff,
    const hdiff_TCompress *compressPlugin = 0,
    size_t kMatchBlockSize = kMatchBlockSize_default,
    const hdiff_TMTSets_s *mtsets = 0);

bool check_compressed_diff(const unsigned char *newData,
                           const unsigned char *newData_end,
                           const unsigned char *oldData,
                           const unsigned char *oldData_end,
                           const unsigned char *diff,
                           const unsigned char *diff_end,
                           hpatch_TDecompress *decompressPlugin);
bool check_compressed_diff(const hpatch_TStreamInput *newData,
                           const hpatch_TStreamInput *oldData,
                           const hpatch_TStreamInput *compressed_diff,
                           hpatch_TDecompress *decompressPlugin);

void resave_compressed_diff(const hpatch_TStreamInput *in_diff,
                            hpatch_TDecompress *decompressPlugin,
                            const hpatch_TStreamOutput *out_diff,
                            const hdiff_TCompress *compressPlugin,
                            hpatch_StreamPos_t out_diff_curPos = 0);

void create_single_compressed_diff(
    const unsigned char *newData, const unsigned char *newData_end,
    const unsigned char *oldData, const unsigned char *oldData_end,
    std::vector<unsigned char> &out_diff,
    const hdiff_TCompress *compressPlugin = 0,
    int kMinSingleMatchScore = kMinSingleMatchScore_default,
    size_t patchStepMemSize = kDefaultPatchStepMemSize,
    bool isUseBigCacheMatch = false, ICoverLinesListener *listener = 0,
    size_t threadNum = 1);

void create_single_compressed_diff_stream(
    const hpatch_TStreamInput *newData, const hpatch_TStreamInput *oldData,
    const hpatch_TStreamOutput *out_diff,
    const hdiff_TCompress *compressPlugin = 0,
    size_t kMatchBlockSize = kMatchBlockSize_default,
    size_t patchStepMemSize = kDefaultPatchStepMemSize,
    const hdiff_TMTSets_s *mtsets = 0);

bool check_single_compressed_diff(const unsigned char *newData,
                                  const unsigned char *newData_end,
                                  const unsigned char *oldData,
                                  const unsigned char *oldData_end,
                                  const unsigned char *diff,
                                  const unsigned char *diff_end,
                                  hpatch_TDecompress *decompressPlugin);
bool check_single_compressed_diff(const hpatch_TStreamInput *newData,
                                  const hpatch_TStreamInput *oldData,
                                  const hpatch_TStreamInput *diff,
                                  hpatch_TDecompress *decompressPlugin);

void resave_single_compressed_diff(
    const hpatch_TStreamInput *in_diff, hpatch_TDecompress *decompressPlugin,
    const hpatch_TStreamOutput *out_diff, const hdiff_TCompress *compressPlugin,
    const hpatch_singleCompressedDiffInfo *diffInfo = 0,
    hpatch_StreamPos_t in_diff_curPos = 0,
    hpatch_StreamPos_t out_diff_curPos = 0);

void get_match_covers_by_block(const hpatch_TStreamInput *newData,
                               const hpatch_TStreamInput *oldData,
                               hpatch_TOutputCovers *out_covers,
                               size_t kMatchBlockSize,
                               const hdiff_TMTSets_s *mtsets);
void get_match_covers_by_block(const unsigned char *newData,
                               const unsigned char *newData_end,
                               const unsigned char *oldData,
                               const unsigned char *oldData_end,
                               hpatch_TOutputCovers *out_covers,
                               size_t kMatchBlockSize, size_t threadNum);

void get_match_covers_by_sstring(
    const unsigned char *newData, const unsigned char *newData_end,
    const unsigned char *oldData, const unsigned char *oldData_end,
    hpatch_TOutputCovers *out_covers,
    int kMinSingleMatchScore = kMinSingleMatchScore_default,
    bool isUseBigCacheMatch = false, ICoverLinesListener *listener = 0,
    size_t threadNum = 1, bool isCanExtendCover = true);
void get_match_covers_by_sstring(
    const unsigned char *newData, const unsigned char *newData_end,
    const unsigned char *oldData, const unsigned char *oldData_end,
    std::vector<hpatch_TCover_sz> &out_covers,
    int kMinSingleMatchScore = kMinSingleMatchScore_default,
    bool isUseBigCacheMatch = false, ICoverLinesListener *listener = 0,
    size_t threadNum = 1, bool isCanExtendCover = true);

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _IS_NEED_CACHE_OLD_BY_COVERS
#define _IS_NEED_CACHE_OLD_BY_COVERS 1
#endif

hpatch_BOOL patch(unsigned char *out_newData, unsigned char *out_newData_end,
                  const unsigned char *oldData,
                  const unsigned char *oldData_end,
                  const unsigned char *serializedDiff,
                  const unsigned char *serializedDiff_end);

hpatch_BOOL
patch_stream(const hpatch_TStreamOutput *out_newData,    // sequential write
             const hpatch_TStreamInput *oldData,         // random read
             const hpatch_TStreamInput *serializedDiff); // random read

hpatch_BOOL patch_stream_with_cache(
    const hpatch_TStreamOutput *out_newData,   // sequential write
    const hpatch_TStreamInput *oldData,        // random read
    const hpatch_TStreamInput *serializedDiff, // random read
    unsigned char *temp_cache, unsigned char *temp_cache_end);

hpatch_BOOL getCompressedDiffInfo(hpatch_compressedDiffInfo *out_diffInfo,
                                  const hpatch_TStreamInput *compressedDiff);
hpatch_inline static hpatch_BOOL
getCompressedDiffInfo_mem(hpatch_compressedDiffInfo *out_diffInfo,
                          const unsigned char *compressedDiff,
                          const unsigned char *compressedDiff_end) {
  hpatch_TStreamInput diffStream;
  mem_as_hStreamInput(&diffStream, compressedDiff, compressedDiff_end);
  return getCompressedDiffInfo(out_diffInfo, &diffStream);
}

hpatch_BOOL
patch_decompress(const hpatch_TStreamOutput *out_newData,   // sequential write
                 const hpatch_TStreamInput *oldData,        // random read
                 const hpatch_TStreamInput *compressedDiff, // random read
                 hpatch_TDecompress *decompressPlugin);

hpatch_BOOL patch_decompress_with_cache(
    const hpatch_TStreamOutput *out_newData,   // sequential write
    const hpatch_TStreamInput *oldData,        // random read
    const hpatch_TStreamInput *compressedDiff, // random read
    hpatch_TDecompress *decompressPlugin, unsigned char *temp_cache,
    unsigned char *temp_cache_end);

hpatch_BOOL patch_decompress_mem(unsigned char *out_newData,
                                 unsigned char *out_newData_end,
                                 const unsigned char *oldData,
                                 const unsigned char *oldData_end,
                                 const unsigned char *compressedDiff,
                                 const unsigned char *compressedDiff_end,
                                 hpatch_TDecompress *decompressPlugin);

typedef struct hpatch_TCoverList {
  hpatch_TCovers *ICovers;
  // private:
  unsigned char _buf[hpatch_kStreamCacheSize * 4];
} hpatch_TCoverList;

void hpatch_coverList_init(hpatch_TCoverList *coverList);

hpatch_BOOL
hpatch_coverList_open_serializedDiff(hpatch_TCoverList *out_coverList,
                                     const hpatch_TStreamInput *serializedDiff);

hpatch_BOOL
hpatch_coverList_open_compressedDiff(hpatch_TCoverList *out_coverList,
                                     const hpatch_TStreamInput *compressedDiff,
                                     hpatch_TDecompress *decompressPlugin);

hpatch_BOOL hpatch_coverList_close(hpatch_TCoverList *coverList);

hpatch_BOOL patch_single_stream(
    sspatch_listener_t *listener,
    const hpatch_TStreamOutput *out_newData, // sequential write
    const hpatch_TStreamInput *oldData,      // random read
    const hpatch_TStreamInput
        *singleCompressedDiff, // sequential read every byte
    hpatch_StreamPos_t
        diffInfo_pos, // default 0, begin pos in singleCompressedDiff
    sspatch_coversListener_t
        *coversListener // default NULL, call by on got covers
);

hpatch_BOOL patch_single_stream_mem(
    sspatch_listener_t *listener, unsigned char *out_newData,
    unsigned char *out_newData_end, const unsigned char *oldData,
    const unsigned char *oldData_end, const unsigned char *diff,
    const unsigned char *diff_end, sspatch_coversListener_t *coversListener);

hpatch_BOOL
getSingleCompressedDiffInfo(hpatch_singleCompressedDiffInfo *out_diffInfo,
                            const hpatch_TStreamInput *singleCompressedDiff,
                            hpatch_StreamPos_t diffInfo_pos);

hpatch_inline static hpatch_BOOL
getSingleCompressedDiffInfo_mem(hpatch_singleCompressedDiffInfo *out_diffInfo,
                                const unsigned char *singleCompressedDiff,
                                const unsigned char *singleCompressedDiff_end) {
  hpatch_TStreamInput diffStream;
  mem_as_hStreamInput(&diffStream, singleCompressedDiff,
                      singleCompressedDiff_end);
  return getSingleCompressedDiffInfo(out_diffInfo, &diffStream, 0);
}

hpatch_BOOL patch_single_compressed_diff(
    const hpatch_TStreamOutput *out_newData,         // sequential write
    const hpatch_TStreamInput *oldData,              // random read
    const hpatch_TStreamInput *singleCompressedDiff, // sequential read
    hpatch_StreamPos_t
        diffData_pos, // diffData begin pos in singleCompressedDiff
    hpatch_StreamPos_t uncompressedSize, hpatch_StreamPos_t compressedSize,
    hpatch_TDecompress *decompressPlugin, hpatch_StreamPos_t coverCount,
    hpatch_size_t stepMemSize, unsigned char *temp_cache,
    unsigned char *temp_cache_end,
    sspatch_coversListener_t
        *coversListener // default NULL, call by on got covers
);

hpatch_BOOL compressed_stream_as_uncompressed(
    hpatch_TUncompresser_t *uncompressedStream,
    hpatch_StreamPos_t uncompressedSize, hpatch_TDecompress *decompressPlugin,
    const hpatch_TStreamInput *compressedStream,
    hpatch_StreamPos_t compressed_pos, hpatch_StreamPos_t compressed_end);
void close_compressed_stream_as_uncompressed(
    hpatch_TUncompresser_t *uncompressedStream);

hpatch_BOOL patch_single_stream_diff(
    const hpatch_TStreamOutput *out_newData,         // sequential write
    const hpatch_TStreamInput *oldData,              // random read
    const hpatch_TStreamInput *uncompressedDiffData, // sequential read
    hpatch_StreamPos_t
        diffData_pos, // diffData begin pos in uncompressedDiffData
    hpatch_StreamPos_t
        diffData_posEnd, // diffData end pos in uncompressedDiffData
    hpatch_StreamPos_t coverCount, hpatch_size_t stepMemSize,
    unsigned char *temp_cache, unsigned char *temp_cache_end,
    sspatch_coversListener_t *coversListener);

#ifdef __cplusplus
}
#endif

#endif
