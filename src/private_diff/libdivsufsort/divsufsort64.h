#ifndef _DIVSUFSORT64_H
#define _DIVSUFSORT64_H 1

#if defined(__cplusplus) ||                                                    \
    (defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L) /* C99 */)
#include <stdint.h> //for uint8_t,int32_t
#else
#if (_MSC_VER >= 1300)
typedef unsigned __int8 uint8_t;
typedef signed __int32 int32_t;
#else
typedef unsigned char uint8_t;
typedef signed int int32_t;
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
#ifndef PRId32
#define PRId32 "d"
#endif
#ifdef _MSC_VER
typedef signed __int64 llong_t;
#ifndef PRId64
#define PRId64 "I64d"
#endif
#else
typedef signed long long llong_t;
#ifndef PRId64
#define PRId64 "lld"
#endif
#endif

#ifndef DIVSUFSORT_API
#ifdef DIVSUFSORT_BUILD_DLL
#define DIVSUFSORT_API
#else
#define DIVSUFSORT_API
#endif
#endif

/*- Datatypes -*/
#ifndef SAUCHAR_T
#define SAUCHAR_T
typedef uint8_t sauchar_t;
#endif /* SAUCHAR_T */
#ifndef SAINT_T
#define SAINT_T
typedef int32_t saint_t;
#endif /* SAINT_T */
#ifndef SAIDX64_T
#define SAIDX64_T
typedef llong_t saidx64_t;
#endif /* SAIDX64_T */

/*- Prototypes -*/

/**
 * Constructs the suffix array of a given string.
 * @param T[0..n-1] The input string.
 * @param SA[0..n-1] The output array of suffixes.
 * @param n The length of the given string.
 * @return 0 if no error occurred, -1 or -2 otherwise.
 */
DIVSUFSORT_API
saint_t divsufsort64(const sauchar_t *T, saidx64_t *SA, saidx64_t n,
                     int threadNum);

/**
 * Returns the version of the divsufsort library.
 * @return The version number string.
 */
DIVSUFSORT_API
const char *divsufsort64_version(void);

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* _DIVSUFSORT64_H */