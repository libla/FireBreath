#ifndef __CSTDINT_H__
#define __CSTDINT_H__

#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901)
#  include <stdint.h>
#elif defined(__GLIBC__) && defined(__GLIBC_HAVE_LONG_LONG)
#  include <inttypes.h>
#else
typedef signed char     int8_t;
typedef signed char     int_least8_t;
typedef signed char     int_fast8_t;
typedef unsigned char   uint8_t;
typedef unsigned char   uint_least8_t;
typedef unsigned char   uint_fast8_t;
typedef short           int16_t;
typedef short           int_least16_t;
typedef short           int_fast16_t;
typedef unsigned short  uint16_t;
typedef unsigned short  uint_least16_t;
typedef unsigned short  uint_fast16_t;
typedef int             int32_t;
typedef int             int_least32_t;
typedef int             int_fast32_t;
typedef unsigned int    uint32_t;
typedef unsigned int    uint_least32_t;
typedef unsigned int    uint_fast32_t;
typedef __int64             intmax_t;
typedef unsigned __int64    uintmax_t;
typedef __int64             int64_t;
typedef __int64             int_least64_t;
typedef __int64             int_fast64_t;
typedef unsigned __int64    uint64_t;
typedef unsigned __int64    uint_least64_t;
typedef unsigned __int64    uint_fast64_t;
#endif

#endif
