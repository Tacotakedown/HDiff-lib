#ifndef HDiff_qsort_parallel_h
#define HDiff_qsort_parallel_h
#include "../parallel/parallel_import.h"
#include <algorithm>

#if (_IS_USED_MULTITHREAD)
#include <thread> //if used vc++, need >= vc2012
#endif

#if (_IS_USED_MULTITHREAD)
inline static size_t __index_by_ratio(size_t size, size_t ratio,
                                      size_t ratio_base) {
  return (size_t)(((hpatch_uint64_t)size) * ratio / ratio_base);
}

template <class TValue, class TCmp> struct _sort_parallel_TCmpi {
  inline _sort_parallel_TCmpi(const TValue *begin, TCmp &cmp)
      : _begin(begin), _cmp(cmp) {}
  inline bool operator()(const size_t &x, const size_t &y) const {
    return _cmp(_begin[x], _begin[y]);
  }
  const TValue *_begin;
  TCmp &_cmp;
};

template <class TValue, class TCmp, size_t kSampleSize>
static TValue *_sort_parallel_partition(TValue *begin, TValue *end, TCmp cmp,
                                        size_t leftWeight = 1,
                                        size_t rightWeight = 1) {
  const size_t size = end - begin;
  size_t samples[kSampleSize];
  const size_t _kIndexStep = size / kSampleSize + 1;
  size_t curIndex = 0;
  for (size_t i = 0; i < kSampleSize; i++, curIndex += _kIndexStep) {
    size_t ri = (curIndex + (size_t)rand()) % size; // muti thread safe
    samples[i] = ri;
  }
  const bool _kIsSortNotNth = false;
  if (_kIsSortNotNth)
    std::sort(samples, samples + kSampleSize,
              _sort_parallel_TCmpi<TValue, TCmp>(begin, cmp));
  size_t _pivot_i =
      __index_by_ratio(kSampleSize, leftWeight, (leftWeight + rightWeight));
  if (!_kIsSortNotNth)
    std::nth_element(samples, samples + _pivot_i, samples + kSampleSize,
                     _sort_parallel_TCmpi<TValue, TCmp>(begin, cmp));
  size_t pivot = samples[_pivot_i];
  std::swap(begin[0], begin[pivot]);
  TValue x(begin[0]);
  size_t mid = 0;
  for (size_t i = mid + 1; i < size; ++i) {
    if (cmp(begin[i], x))
      std::swap(begin[++mid], begin[i]);
  }
  std::swap(begin[0], begin[mid]);
  return begin + mid;
}

template <class TValue, class TCmp, size_t kSampleSize>
static void _sort_parallel_thread(TValue *begin, TValue *end, TCmp cmp,
                                  size_t threadNum) {
  if (threadNum > 1) {
    const size_t rightWeight = (threadNum >> 1);
    const size_t leftWeight = threadNum - rightWeight;
    TValue *mid;
    const bool _kIsPartitionNotMerge = true;
    if (_kIsPartitionNotMerge) { // partition
      // mid=begin+__index_by_ratio(size,leftWeight,threadNum);
      // std::nth_element(begin,mid,end,cmp); //for test
      // mid=std::_Partition_by_median_guess_unchecked(begin, end, cmp).first;
      // //for test by vc
      mid = _sort_parallel_partition<TValue, TCmp, kSampleSize>(
          begin, end, cmp, leftWeight, rightWeight);
    } else {
      mid = begin + __index_by_ratio(end - begin, leftWeight, threadNum);
    }

    std::thread threadRight(_sort_parallel_thread<TValue, TCmp, kSampleSize>,
                            mid, end, cmp, rightWeight);
    _sort_parallel_thread<TValue, TCmp, kSampleSize>(begin, mid, cmp,
                                                     leftWeight);
    threadRight.join();

    if (!_kIsPartitionNotMerge) { // merge
      std::inplace_merge(begin, mid, end, cmp);
    }
  } else {
    std::sort(begin, end, cmp);
    // printf("parallel sort size: %" PRIu64 "
    // \n",(hpatch_StreamPos_t)(end-begin));
  }
}
#endif

template <class TValue, class TCmp, size_t kMinQSortParallelSize,
          size_t kSampleSize>
static void sort_parallel(TValue *begin, TValue *end, TCmp cmp,
                          size_t threadNum) {
#if (_IS_USED_MULTITHREAD)
  const size_t size = end - begin;
  if ((threadNum > 1) && (size >= kMinQSortParallelSize)) {
    const size_t maxThreanNum = size / (kMinQSortParallelSize / 2);
    threadNum = (threadNum <= maxThreanNum) ? threadNum : maxThreanNum;
    // std::random_shuffle(begin,end); //test shuffle befor parallel sort?
    _sort_parallel_thread<TValue, TCmp, kSampleSize>(begin, end, cmp,
                                                     threadNum);
  } else
#endif
  {
    std::sort(begin, end, cmp);
  }
}

#endif