#ifndef __BYTES_RLE_H_
#define __BYTES_RLE_H_
#include "../patch_types.h" //hpatch_TStreamInput
#include <string.h>
#include <vector>

namespace hdiff_private {

#ifndef kMaxBytesRleLen
static const size_t kMaxBytesRleLen = (size_t)(((size_t)1 << 31) - 1);
#endif

enum TRleParameter {
  kRle_bestSize = 1,
  kRle_default = 3,
  kRle_bestUnRleSpeed = 32
};

void bytesRLE_save(std::vector<unsigned char> &out_code,
                   const unsigned char *src, const unsigned char *src_end,
                   int rle_parameter = kRle_default);
void bytesRLE_save(std::vector<unsigned char> &out_code,
                   const hpatch_TStreamInput *src, // sequential read
                   int rle_parameter = kRle_default);

void bytesRLE_save(std::vector<unsigned char> &out_ctrlBuf,
                   std::vector<unsigned char> &out_codeBuf,
                   const unsigned char *src, const unsigned char *src_end,
                   int rle_parameter);
void bytesRLE_save(std::vector<unsigned char> &out_ctrlBuf,
                   std::vector<unsigned char> &out_codeBuf,
                   const hpatch_TStreamInput *src, // sequential read
                   int rle_parameter);

struct TSingleStreamRLE0 {
  std::vector<unsigned char> fixed_code;
  std::vector<unsigned char> uncompressData;
  hpatch_StreamPos_t len0;
  inline TSingleStreamRLE0() : len0(0) {}
  inline hpatch_StreamPos_t curCodeSize() const { return maxCodeSize(0, 0); }
  hpatch_StreamPos_t maxCodeSize(const unsigned char *appendData,
                                 const unsigned char *appendData_end) const;
  hpatch_StreamPos_t
  maxCodeSize(const hpatch_TStreamInput *appendData) const; // sequential read
  hpatch_StreamPos_t
  maxCodeSizeByZeroLen(hpatch_StreamPos_t appendZeroLen) const;
  void append(const unsigned char *appendData,
              const unsigned char *appendData_end);
  void append(const hpatch_TStreamInput *appendData); // sequential read
  void appendByZeroLen(hpatch_StreamPos_t appendZeroLen);
  void finishAppend();
  inline void clear() {
    fixed_code.clear();
    uncompressData.clear();
    len0 = 0;
  }
};

} // namespace hdiff_private
#endif //__BYTES_RLE_H_