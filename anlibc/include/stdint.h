/*
    stdint.h
    Aean, 2014.6.29
    This file is part of anlibc, which itself is part of ancc project.
    ancc project is licensed under the terms of the MIT License.
    Copyright (C) 2014 Aean
*/

#ifndef __ANLIBC_STDINT_INCLUDED_
#define __ANLIBC_STDINT_INCLUDED_

typedef signed char          int8_t;
typedef signed short int     int16_t;
typedef signed long int      int32_t;
typedef signed long long int int64_t;

typedef unsigned char          uint8_t;
typedef unsigned short int     uint16_t;
typedef unsigned long int      uint32_t;
typedef unsigned long long int uint64_t;

typedef signed char          int_least8_t;
typedef signed short int     int_least16_t;
typedef signed long int      int_least32_t;
typedef signed long long int int_least64_t;

typedef unsigned char          uint_least8_t;
typedef unsigned short int     uint_least16_t;
typedef unsigned long int      uint_least32_t;
typedef unsigned long long int uint_least64_t;

typedef signed long int      int_fast8_t;
typedef signed long int      int_fast16_t;
typedef signed long int      int_fast32_t;
typedef signed long long int int_fast64_t;

typedef unsigned long int      uint_fast8_t;
typedef unsigned long int      uint_fast16_t;
typedef unsigned long int      uint_fast32_t;
typedef unsigned long long int uint_fast64_t;

typedef signed long int   intptr_t;
typedef unsigned long int uintptr_t;

typedef signed long long int   intmax_t;
typedef unsigned long long int uintmax_t;

#define INT8_MIN (-128)
#define INT16_MIN (-32768)
#define INT32_MIN (-2147483647L - 1)
#define INT64_MIN  (-9223372036854775807LL - 1)

#define INT8_MAX 127
#define INT16_MAX 32767
#define INT32_MAX 2147483647L
#define INT64_MAX 9223372036854775807LL

#define UINT8_MAX 255
#define UINT16_MAX 65535
#define UINT32_MAX 4294967295UL
#define UINT64_MAX 18446744073709551615ULL

#define INT_LEAST8_MIN INT8_MIN
#define INT_LEAST16_MIN INT16_MIN
#define INT_LEAST32_MIN INT32_MIN
#define INT_LEAST64_MIN INT64_MIN

#define INT_LEAST8_MAX INT8_MAX
#define INT_LEAST16_MAX INT16_MAX
#define INT_LEAST32_MAX INT32_MAX
#define INT_LEAST64_MAX INT64_MAX

#define UINT_LEAST8_MAX UINT8_MAX
#define UINT_LEAST16_MAX UINT16_MAX
#define UINT_LEAST32_MAX UINT32_MAX
#define UINT_LEAST64_MAX UINT64_MAX

#define INT_FAST8_MIN INT32_MIN
#define INT_FAST16_MIN INT32_MIN
#define INT_FAST32_MIN INT32_MIN
#define INT_FAST64_MIN INT64_MIN

#define INT_FAST8_MAX INT32_MAX
#define INT_FAST16_MAX INT32_MAX
#define INT_FAST32_MAX INT32_MAX
#define INT_FAST64_MAX INT64_MAX

#define UINT_FAST8_MAX UINT32_MAX
#define UINT_FAST16_MAX UINT32_MAX
#define UINT_FAST32_MAX UINT32_MAX
#define UINT_FAST64_MAX UINT64_MAX

#define INTPTR_MIN INT32_MIN
#define INTPTR_MAX INT32_MAX
#define UINTPTR_MAX UINT32_MAX

#define INTMAX_MIN INT64_MIN
#define INTMAX_MAX INT64_MAX
#define UINTMAX_MAX UINT64_MAX

#define PTRDIFF_MIN INT32_MIN
#define PTRDIFF_MAX INT32_MAX

#define SIZE_MAX UINT32_MAX

#define INT8_C(val) (INT_LEAST8_MAX-INT_LEAST8_MAX+(val))
#define INT16_C(val) (INT_LEAST16_MAX-INT_LEAST16_MAX+(val))
#define INT32_C(val) (INT_LEAST32_MAX-INT_LEAST32_MAX+(val))
#define INT64_C(val) val##LL

#define UINT8_C(val) (val)
#define UINT16_C(val) (val)
#define UINT32_C(val) (val##U)
#define UINT64_C(val) val##ULL

#define INTMAX_C(val) val##LL
#define UINTMAX_C(val) val##ULL


#endif /* Guard word. */
