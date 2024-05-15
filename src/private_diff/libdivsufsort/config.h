#ifndef _CONFIG_H
#define _CONFIG_H 1

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/** Define to the version of this package. **/
#define PROJECT_VERSION_FULL "2.0.1-14-g5f60d6f"

/** Define to 1 if you have the header files. **/
#define HAVE_INTTYPES_H 0
#define HAVE_STDDEF_H 1
#if defined(__cplusplus) ||                                                    \
    (defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L) /* C99 */)
#define HAVE_STDINT_H 1
#else
#define HAVE_STDINT_H 0
#endif
#define HAVE_STDLIB_H 1
#define HAVE_STRING_H 1
#define HAVE_STRINGS_H 0
#define HAVE_MEMORY_H 1
#define HAVE_SYS_TYPES_H 0

/** for WinIO **/
/* #undef HAVE_IO_H */
/* #undef HAVE_FCNTL_H */
/* #undef HAVE__SETMODE */
/* #undef HAVE_SETMODE */
/* #undef HAVE__FILENO */
/* #undef HAVE_FOPEN_S */
/* #undef HAVE__O_BINARY */
#ifndef HAVE__SETMODE
#if HAVE_SETMODE
#define _setmode setmode
#define HAVE__SETMODE 1
#endif
#if HAVE__SETMODE && !HAVE__O_BINARY
#define _O_BINARY 0
#define HAVE__O_BINARY 1
#endif
#endif

/** for inline **/
#ifndef INLINE
#ifdef _MSC_VER
#define INLINE __inline
#else
#define INLINE inline
#endif
#endif

/** for VC++ warning **/
#ifdef _MSC_VER
#pragma warning(disable : 4127)
#endif

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* _CONFIG_H */