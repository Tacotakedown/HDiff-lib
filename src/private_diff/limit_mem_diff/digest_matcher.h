#ifndef digest_matcher_h
#define digest_matcher_h
#include "../mem_buf.h"
#include "adler_roll.h"
#include "bloom_filter.h"
#include "covers.h"

namespace hdiff_private {

typedef uint64_t adler_uint_t;
#define adler_start fast_adler64_start
#define adler_roll fast_adler64_roll
typedef uint64_t adler_hash_t;
static inline adler_hash_t adler_to_hash(const uint64_t x) { return x; }

// typedef adler128_t      adler_uint_t;
//#define adler_start     fast_adler128_start
//#define adler_roll      fast_adler128_roll
// typedef uint64_t        adler_hash_t;
// static inline adler_hash_t adler_to_hash(const adler128_t& x){ return
// x.adler^x.sum; } static inline bool operator !=(const adler128_t& x,const
// adler128_t& y){
//    return (x.adler!=y.adler)||(x.sum!=y.sum);}
// static inline bool operator <(const adler128_t& x,const adler128_t& y){
//    if (x.adler!=y.adler) return (x.adler<y.adler); else return (x.sum<y.sum);
//    }

// typedef uint32_t        adler_uint_t;
//#define adler_start     fast_adler32_start
//#define adler_roll      fast_adler32_roll
// typedef uint32_t        adler_hash_t;
// static inline adler_hash_t adler_to_hash(const uint32_t x){ return x; }

class TDigestMatcher {
public:
  // throw std::runtime_error when data->read error or kMatchBlockSize error;
  TDigestMatcher(const hpatch_TStreamInput *oldData,
                 const hpatch_TStreamInput *newData, size_t kMatchBlockSize,
                 const hdiff_TMTSets_s &mtsets);
  void search_cover(hpatch_TOutputCovers *out_covers);
  ~TDigestMatcher();

private:
  TDigestMatcher(const TDigestMatcher &);            // empty
  TDigestMatcher &operator=(const TDigestMatcher &); // empty
private:
  const hpatch_TStreamInput *m_oldData;
  const hpatch_TStreamInput *m_newData;
  std::vector<adler_uint_t> m_blocks;
  TBloomFilter<adler_hash_t> m_filter;
  std::vector<uint32_t> m_sorted_limit;
  std::vector<size_t> m_sorted_larger;
  bool m_isUseLargeSorted;
  const hdiff_TMTSets_s m_mtsets;
  TAutoMem m_mem;
  size_t m_newCacheSize;
  size_t m_oldCacheSize;
  size_t m_oldMinCacheSize;
  size_t m_backupCacheSize;
  size_t m_kMatchBlockSize;

  void getDigests();
  size_t getSearchThreadNum() const;
  void _search_cover(const hpatch_TStreamInput *newData,
                     hpatch_StreamPos_t newOffset,
                     hpatch_TOutputCovers *out_covers, unsigned char *pmem,
                     void *dataLocker = 0, void *newDataLocker = 0);

public: // private for muti-thread
  void _search_cover_thread(hpatch_TOutputCovers *out_covers,
                            unsigned char *pmem, void *mt_data);
};

} // namespace hdiff_private
#endif