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

void *memcpy(void * __ANCC_RESTRICT dst, const void * __ANCC_RESTRICT src, size_t n)
{
	char *dst_c;
	const char *src_c;
	uint32_t *dst_u;
	const uint32_t *src_u;
	ptrdiff_t i;

	dst_u = (uint32_t*)dst;
	src_u = (const uint32_t*)src;
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

	return dst;
}

void *memmove(void *dst, const void *src, size_t n)
{
    char* dst_c = dst;
    const char* src_c = src;

    if (dst_c <= src_c || dst_c >= (src_c + n))
        memcpy(dst, src, n);
    else
        while (n--)
            *dst_c-- = *src_c--;

    return dst;
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

    cp = (char*)lp;
    csp = (const char*)sp;  /* Copy redundant bytes one by one. */
    while ((*cp++ = *csp++));

	return dst;
}

char* strncpy(char* __ANCC_RESTRICT dst, const char* __ANCC_RESTRICT src, size_t n)
{
        char *start = dst;

        while (n-- && (*dst++ = *src++));    /* copy string */

        if (n)                              /* pad out with zeroes */
            while (--n)
                *dst++ = '\0';

        return(start);
}

char* strcat(char* __ANCC_RESTRICT dst, const char* __ANCC_RESTRICT src)
{
	strcpy(dst + strlen(dst), src); /* Reuse strlen & strcpy. */
	return dst;
}

char* strncat(char* __ANCC_RESTRICT dst, const char* __ANCC_RESTRICT src, size_t n)
{
	size_t ls = strlen(src);
	size_t ld = strlen(dst);
	if (n > ls){
        memcpy(dst + ld, src, n);
        *(dst + ld + n) = '\0';
    }else{
        memcpy(dst + ld, src, ls);
    }
	return dst;
}

int memcmp(const void *s1, const void *s2, size_t n)
{
    const unsigned char *l = s1;
    const unsigned char *r = s2;
    while(n--){
        if (*l != *r)
            return ((signed int)*l - (signed int)*r);
        l++;
        r++;
    }
    return 0;
}

int strcmp(const char *s1, const char *s2)
{
    while(*s1 && *s2){
        if (*s1 != *s2) return ((signed int)*s1 - (signed int)*s2);
        s1++;
        s2++;
    }
    return ((signed int)*s1 - (signed int)*s2);
}

int strncmp(const char *s1, const char *s2, size_t n)
{
    while(n-- && *s1 && *s2){
        if (*s1 != *s2) return ((signed int)*s1 - (signed int)*s2);
        s1++;
        s2++;
    }
    return n ? ((signed int)*s1 - (signed int)*s2) : 0;
}

void* memchr(const void* ptr, int _ch, size_t n)
{
    const unsigned char* _str = ptr;
	unsigned char ch = (unsigned char)_ch;
	while (n--){
		if (*_str == ch) return (void*)_str;
		_str++;
	}
	return NULL;
}

char* strchr(const char* _str, int _ch)
{
	char ch = _ch & 0xff;
	uint32_t mask = ch;
	const uint32_t* i32str = (const uint32_t*)_str;

	/* Fill a mask with ch. */
	mask |= mask << 8;
	mask |= mask << 16;

	/* Pass though while \0 or ch not found. */
	while (!(haszerobyte(*i32str) || haszerobyte(*i32str^mask)))
		i32str++;

	/* Find the position of first \0 or ch. */
	_str = (const char*)i32str;
	if (*_str == ch) return (char*)_str;
	else if (*_str == 0) return NULL;
	_str++;
	if (*_str == ch) return (char*)_str;
	else if (*_str == 0) return NULL;
	_str++;
	if (*_str == ch) return (char*)_str;
	else if (*_str == 0) return NULL;
	_str++;
	if (*_str == ch) return (char*)_str;
	else if (*_str == 0) return NULL;
	_str++;
	if (*_str == ch) return (char*)_str;
	else if (*_str == 0) return NULL;
	_str++;
	if (*_str == ch) return (char*)_str;
	else if (*_str == 0) return NULL;
	_str++;
	if (*_str == ch) return (char*)_str;
	else if (*_str == 0) return NULL;
	_str++;
	return *_str ? (char*)_str : NULL;
}

char* strstr(const char* _str, const char* _ch)
{

	/*
		Brute Force Algorithm - the most silly algorithm with least
		preprocessing / extra memory spending. Compare each byte and
		shift by exactly 1 byte on fails.
		This algorithm won't cause memory exceptions, and really easy
		to write in such a data parallel manner.
	*/

	const uint32_t* i32str_p = (const uint32_t*)_str;
	const uint32_t* i32ch_p = (const uint32_t*)_ch;
	const char *cstr_p, *cch_p;
	size_t needle_len, i;
	uint32_t i32reg;
	int ok = *_str == *_ch; /* Check if _ch is prefix of _str while looking for string tail. */

	/* Do an init check may avoid wasted prefix check. */
	if (ok) {
		/* Check prefix. */
		while (!(haszerobyte(*i32str_p) || haszerobyte(*i32ch_p)))
			ok = ok & (*i32str_p++ == *i32ch_p++);
		cstr_p = (const char*)i32str_p;
		cch_p = (const char*)i32ch_p;
		while (*cstr_p && *cch_p)
			ok = ok & (*cstr_p++ == *cch_p++);

		if (*cch_p) return (char*)cstr_p; /* If reached end of _str first, it is unable to match. */
		if (ok) return (char*)_str; /* If found end of _ch, and still ok, it's prefix of _str. */
	}else{
		/* Don't check prefix. */
		while (!(haszerobyte(*i32str_p) || haszerobyte(*i32ch_p)))
			i32str_p++, i32ch_p++;
		cstr_p = (const char*)i32str_p;
		cch_p = (const char*)i32ch_p;
		while (*cstr_p && *cch_p)
			cstr_p++, cch_p++;

		if (*cch_p) return (char*)cstr_p; /* If reached end of _str first, it is unable to match. */
	}
	needle_len = cch_p - _ch; /* Now we got length of _ch. */

	if (needle_len > sizeof(uint32_t)){
		longcut:
		_str = strchr(_str+1, *_ch); /* Cut _str's head off. */
		if (!_str) return (char*)_str;
		i32ch_p = (const uint32_t*)_ch;
		i32str_p = (const uint32_t*)_str;
		for (i = needle_len / sizeof(uint32_t); i; i--){ /* One more decrease operate, to avoid branch. */
			if (*i32ch_p ^ *i32str_p) goto longcut;
			i32ch_p++;
			i32str_p++;
		}
		cch_p = (const char*)i32ch_p;
		cstr_p = (const char*)i32str_p;
		for (i = needle_len & (sizeof(uint32_t)-1); i; i--){
			if (*cch_p ^ *cstr_p) goto longcut;
			cch_p++;
			cstr_p++;
		}

		return (char*)_str;
	}else if (needle_len > 1){
	    const uint32_t nl_mask = ~((uint32_t)(-1L) << ((needle_len - 1) * CHAR_BIT));
		i32reg = *(const uint32_t*)(_ch + 1); /* Dirty but fast loading. */
		do{
			_str = strchr(_str + 1, *_ch); /* Cut _str's head off. */
			if (!_str) return (char*)_str;
			i32str_p = (const uint32_t*)(_str + 1);
		} while ((i32reg ^ *i32str_p) & nl_mask);

		return (char*)_str;
	}else{
		/* If _ch size equal to 1 */
		return strchr(_str + 1, *_ch);
	}
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

unsigned int strlen(const char* str)
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

char* strdup(char* s){
    char* newbase = 0;
    newbase = calloc(strlen(s) + 1, 1);
    if (newbase)
        strcpy(newbase, s);
    return newbase;
}
