#ifndef bloom_filter_h
#define bloom_filter_h
#include "../../parallel/parallel_channel.h"
#include <assert.h>
#include <stdexcept> //std::runtime_error
#include <string.h>  //memset

#if (_IS_USED_MULTITHREAD)
#if defined(ANDROID) && (defined(__GNUC__) || defined(__clang__))
#define _IS_USED__sync_fetch_and_or 1
#else
#include <atomic> //need c++11, vc version need vc2012
#endif
#endif

namespace hdiff_private {

class TBitSet {
public:
  inline TBitSet() : m_bits(0), m_bitSize(0) {}
  inline ~TBitSet() { clear(0); }

  inline void set(size_t bitIndex) {
    // assert(bitIndex<m_bitSize);
    m_bits[bitIndex >> kBaseShr] |= ((base_t)1 << (bitIndex & kBaseMask));
  }
#if (_IS_USED_MULTITHREAD)
  inline void set_MT(size_t bitIndex) {
    // assert(bitIndex<m_bitSize);
#if (_IS_USED__sync_fetch_and_or)
    __sync_fetch_and_or(&m_bits[bitIndex >> kBaseShr],
                        ((base_t)1 << (bitIndex & kBaseMask)));
#else
    ((std::atomic<base_t> *)&m_bits[bitIndex >> kBaseShr])
        ->fetch_or(((base_t)1 << (bitIndex & kBaseMask)));
#endif
  }
#endif
  inline bool is_hit(size_t bitIndex) const {
    // assert(bitIndex<m_bitSize);
    return 0 != (m_bits[bitIndex >> kBaseShr] &
                 ((base_t)1 << (bitIndex & kBaseMask)));
  }

  inline size_t bitSize() const { return m_bitSize; }

  void clear(size_t newBitSize) {
    size_t count = bitSizeToCount(newBitSize);
    if (newBitSize == m_bitSize) {
      if (count > 0)
        memset(m_bits, 0, count * sizeof(base_t));
    } else {
      m_bitSize = newBitSize;
      if (m_bits) {
        delete[] m_bits;
        m_bits = 0;
      }
      if (count > 0) {
        m_bits = new base_t[count];
        memset(m_bits, 0, count * sizeof(base_t));
      }
    }
  }

private:
  inline static size_t bitSizeToCount(size_t bitSize) {
    return (bitSize + (kBaseTBits - 1)) / kBaseTBits;
  }
  typedef uint32_t base_t;
  enum {
    kBaseShr = (sizeof(base_t) == 8) ? 6 : ((sizeof(base_t) == 4) ? 5 : 0),
    kBaseTBits = (1 << kBaseShr),
    kBaseMask = kBaseTBits - 1
  };
  // assert(kBaseTBits==sizeof(base_t)*8);
  struct __private_TBitSet_check_base_t_size {
    char _[(kBaseTBits == sizeof(base_t) * 8) ? 1 : -1];
  };
  base_t *m_bits;
  size_t m_bitSize;
};

template <class T> class TBloomFilter {
public:
  enum { kZoomMin = 3, kZoomBig = 32 };

  inline TBloomFilter() : m_bitSetMask(0) {}
  inline void clear() { m_bitSet.clear(0); }
  void init(size_t dataCount, size_t zoom = kZoomBig) {
    m_bitSetMask = getMask(dataCount, zoom); // mask is 2^N-1
    m_bitSet.clear(m_bitSetMask + 1);
  }
  inline size_t bitSize() const { return m_bitSet.bitSize(); }
  inline void insert(T data) {
    m_bitSet.set(hash0(data));
    m_bitSet.set(hash1(data));
    m_bitSet.set(hash2(data));
  }
#if (_IS_USED_MULTITHREAD)
  inline void insert_MT(T data) {
    m_bitSet.set_MT(hash0(data));
    m_bitSet.set_MT(hash1(data));
    m_bitSet.set_MT(hash2(data));
  }
#endif
  inline bool is_hit(T data) const {
    return m_bitSet.is_hit(hash0(data)) && m_bitSet.is_hit(hash1(data)) &&
           m_bitSet.is_hit(hash2(data));
  }

private:
  TBitSet m_bitSet;
  size_t m_bitSetMask;
  static size_t getMask(size_t dataCount, size_t zoom) {
    if (zoom < kZoomMin)
      throw std::runtime_error("TBloomFilter::getMask() zoom too small error!");
    size_t bitSize = dataCount * zoom;
    if ((bitSize / zoom) != dataCount)
      throw std::runtime_error(
          "TBloomFilter::getMask() bitSize too large error!");
    unsigned int bit = 10;
    while ((((size_t)1 << bit) < bitSize) && (bit < sizeof(size_t) * 8 - 1))
      ++bit;
    return ((size_t)1 << bit) - 1;
  }

  inline size_t hash0(T key) const {
    return (key ^ (key >> (sizeof(T) * 4))) & m_bitSetMask;
  }
  inline size_t hash1(T key) const {
    return ((~key) + (key << (sizeof(T) * 2 + 1)) + 1) % m_bitSetMask;
  }
  inline size_t hash2(T key) const {
    size_t h = (sizeof(T) > 4) ? _hash2_64(key) : _hash2_32((size_t)key);
    return h % (m_bitSetMask - 1);
  }
  static size_t
  _hash2_32(size_t key) { // from: https://gist.github.com/badboy/6267743
    const size_t c2 = 0x27d4eb2d; // a prime or an odd constant
    key = (key ^ 61) ^ (key >> 16);
    key = key + (key << 3);
    key = key ^ (key >> 4);
    key = key * c2;
    key = key ^ (key >> 15);
    return key;
  }
  static size_t _hash2_64(T key) {
    key = (~key) + (key << 18); // key = (key << 18) - key - 1;
    key = key ^ (key >> 31);
    key = key * 21; // key = (key + (key << 2)) + (key << 4);
    key = key ^ (key >> 11);
    key = key + (key << 6);
    key = key ^ (key >> 22);
    return (size_t)key;
  }
};

} // namespace hdiff_private
#endif /* bloom_filter_h */