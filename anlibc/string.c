/*
    string.c
    Aean, 2014.6.29
    This file is part of anlibc, which itself is part of ancc project.
    ancc project is licensed under the terms of the MIT License.
    Copyright (C) 2014 Aean & HUST
*/

#include "anlibc.h"

/* Bithack to check zero byte existance in uint64_t type. */
#define haszerobyte(v) (((v) - 0x01010101UL) & ~(v) & 0x80808080UL)

void *memcpy(void * __ANCC_RESTRICT s1, const void * __ANCC_RESTRICT s2, size_t n)
{
	char *dst_c;
	const char *src_c;
	uint32_t *dst_u;
	const uint32_t *src_u;
	ptrdiff_t i;

	dst_u = (uint32_t*)s1;
	src_u = (const uint32_t*)s2;
	i = (n / sizeof(uint32_t)) - 7;
	/* Loop unroll. */
	for (; i > 0; i -= 8){
		*dst_u++ = *src_u++; /* 0 */
		*dst_u++ = *src_u++; /* 1 */
		*dst_u++ = *src_u++; /* 2 */
		*dst_u++ = *src_u++; /* 3 */
		*dst_u++ = *src_u++; /* 4 */
		*dst_u++ = *src_u++; /* 5 */
		*dst_u++ = *src_u++; /* 6 */
		*dst_u++ = *src_u++; /* 7 */
	}

	for (i += 7; i > 0; --i)
		*dst_u++ = *src_u++; /* 0 */

	i = n % sizeof(uint32_t);
	dst_c = (char*)dst_u;
	src_c = (const char*)src_u;

	while (i-- > 0)
		*dst_c++ = *src_c++;

	return s1;
}

void *memmove(void *s1, const void *s2, size_t n)
{
    void* tmps = alloca(n);
    memcpy(tmps, s2, n);
    memcpy(s1, tmps, n);
    return s1;
}

char* strcpy(char* __ANCC_RESTRICT dst, const char* __ANCC_RESTRICT src)
{
	uint32_t* lp = (uint32_t*)dst; /* Unrolling by 4. */
	const uint32_t* sp = (const uint32_t*)src;
	char* cp;
	const char* csp;

	while (!haszerobyte(*sp)){  /* Bit hack. */
		*lp++ = *sp++;
	}

	if (*lp & 0x7f000000UL){     /* If not divisible by 4; */
		cp = (char*)lp;
		csp = (const char*)sp;  /* Copy redundant bytes one by one. */
		while ((*cp++ = *csp++));
	}
	return dst;
}

void* memset(void* _dst, int _val, size_t _size)
{
	char* dst_c;
	uint32_t* dst_u;
	ptrdiff_t i;

	/* Fill ch with _val. */
	char cch = _val & 0xff;
	uint32_t ch = cch;
	ch |= ch << 8;
	ch |= ch << 16;

	dst_u = (uint32_t*)_dst;
	i = (_size / sizeof(uint32_t)) - 7;
	/* Loop unroll. */
	for (; i > 0; i -= 8){
		*dst_u++ = ch; /* 0 */
		*dst_u++ = ch; /* 1 */
		*dst_u++ = ch; /* 2 */
		*dst_u++ = ch; /* 3 */
		*dst_u++ = ch; /* 4 */
		*dst_u++ = ch; /* 5 */
		*dst_u++ = ch; /* 6 */
		*dst_u++ = ch; /* 7 */
	}

	for (i += 7; i > 0; --i)
		*dst_u++ = ch; /* 0 */

	i = _size % sizeof(uint32_t);
	dst_c = (char*)dst_u;
	while (i-- > 0)
		*dst_c++ = cch;

	return _dst;
}

size_t strlen(const char* str)
{

	const uint32_t* lp = (const uint32_t*)str; /* Unrolling by 4. */

	const char* cp;
	while (!haszerobyte(*lp)) /* Bit hack. */
		lp++;
	cp = (const char*)lp; /* If not divisible by 4; */
	while (*cp)          /* Check zero byte one by one. */
		cp++;
	return (cp - str);
}

