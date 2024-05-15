#ifndef __SUFFIX_STRING_H_
#define __SUFFIX_STRING_H_
#include <stddef.h> //for ptrdiff_t,size_t
#include <vector>

#ifndef _SSTRING_FAST_MATCH
#define _SSTRING_FAST_MATCH 5
#endif
#if (_SSTRING_FAST_MATCH > 0)
#if (_SSTRING_FAST_MATCH < 2)
#error must _SSTRING_FAST_MATCH>=2!
#endif
#include "limit_mem_diff/adler_roll.h"
#include "limit_mem_diff/bloom_filter.h"

#endif

#if defined(__cplusplus) ||                                                    \
    (defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L) /* C99 */)
#include <stdint.h> //for int32_t
namespace hdiff_private {
#else
namespace hdiff_private {
#if (_MSC_VER >= 1300)
typedef signed __int32 int32_t;
#else
typedef signed int int32_t;
#endif
#endif

#if (_SSTRING_FAST_MATCH > 0)
class TFastMatchForSString {
public:
  typedef uint32_t THash;
  typedef unsigned char TChar;
  enum { kFMMinStrSize = _SSTRING_FAST_MATCH };

  inline TFastMatchForSString() {}
  inline void clear() { bf.clear(); }
  void buildMatchCache(const TChar *src_begin, const TChar *src_end,
                       size_t threadNum);

  static inline THash getHash(const TChar *datas) {
    return fast_adler32_start(datas, kFMMinStrSize);
  }
  static inline THash rollHash(THash h, const TChar *cur) {
    return fast_adler32_roll(h, kFMMinStrSize, cur[-kFMMinStrSize], cur[0]);
  }

  inline bool isHit(THash h) const { return bf.is_hit(h); }

private:
  TBloomFilter<THash> bf;
};
#endif

class TSuffixString {
public:
  typedef ptrdiff_t TInt;
  typedef int32_t TInt32;
  typedef unsigned char TChar;
  explicit TSuffixString(bool isUsedFastMatch = false);
  ~TSuffixString();

  // throw std::runtime_error when create SA error
  TSuffixString(const TChar *src_begin, const TChar *src_end,
                bool isUsedFastMatch = false, size_t threadNum = 1);
  void resetSuffixString(const TChar *src_begin, const TChar *src_end,
                         size_t threadNum = 1);

  inline const TChar *src_begin() const { return m_src_begin; }
  inline const TChar *src_end() const { return m_src_end; }
  inline size_t SASize() const { return (size_t)(m_src_end - m_src_begin); }
  void clear();

  inline TInt SA(TInt i) const { // return m_SA[i];//排好序的后缀字符串数组.
    if (isUseLargeSA())
      return m_SA_large[i];
    else
      return (TInt)m_SA_limit[i];
  }
  TInt lower_bound(const TChar *str, const TChar *str_end)
      const; // return index in SA; must str_end-str>=2 !
private:
  TSuffixString(const TSuffixString &);            // empty
  TSuffixString &operator=(const TSuffixString &); // empty
private:
  const TChar *m_src_begin; //原字符串.
  const TChar *m_src_end;
  std::vector<TInt32> m_SA_limit;
  std::vector<TInt> m_SA_large;
  enum { kLimitSASize = (1 << 30) - 1 + (1 << 30) }; // 2G-1
  inline bool isUseLargeSA() const {
    return (sizeof(TInt) > sizeof(TInt32)) && (SASize() > kLimitSASize);
  }

private:
  // all cache for lower_bound speed
  const bool m_isUsedFastMatch;
#if (_SSTRING_FAST_MATCH > 0)
  TFastMatchForSString m_fastMatch; // a big memory cache & build slow
#endif
  const void *m_cached_SA_begin;
  const void *m_cached_SA_end;
  const void *m_cached1char_range[256 + 1];
  void *m_cached2char_range; //[256*256+1]
  typedef TInt (*t_lower_bound_func)(const void *rbegin, const void *rend,
                                     const TChar *str, const TChar *str_end,
                                     const TChar *src_begin,
                                     const TChar *src_end, const void *SA_begin,
                                     size_t min_eq);
  t_lower_bound_func m_lower_bound;
  void build_cache(size_t threadNum);
  void clear_cache();
};

} // namespace hdiff_private
#endif //__SUFFIX_STRING_H_