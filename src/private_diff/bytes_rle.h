#ifndef __BYTES_RLE_H_
#define __BYTES_RLE_H_

#include <vector>

namespace hdiff_private {
enum TRleParameter {
  kRle_bestSize = 1,
  kRle_default = 3,
  kRle_BestUnRleSpeed = 32,
};

void bytesRLE_Save(std::vector<unsigned char> &out_code,
                   const unsigned char *src, const unsigned char *src_end,
                   int rle_parameter = kRle_default);

void bytesRLE_Save(std::vector<unsigned char> &out_ctrlBuf,
                   std::vector<unsigned char> &out_codeBuf,
                   const unsigned char *src, const unsigned char *src_end,
                   int rle_parameter);

} // namespace hdiff_private

#endif // __BYTES_RLE_H