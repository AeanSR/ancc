/*
    assert.h
    Aean, 2014.6.30
    This file is part of anlibc, which itself is part of ancc project.
    ancc project is licensed under the terms of the MIT License.
    Copyright (C) 2014 Aean
*/

#ifndef __ANLIIBC_ASSERT_INCLUDED_
#define __ANLIIBC_ASSERT_INCLUDED_

#ifndef __ANLIBC_INTLBUILD__
#include <stdio.h>
#endif

#ifdef NDEBUG
#define assert(expression) ((void)0)
#else /* NDEBUG */
#define __ANLIBC_STR2(v) #v
#define __ANLIBC_STR(v) __ANLIBC_STR2(v)
#define assert(expression) if(!(expression)){fprintf(stderr, \
        "Assertion failed: %s, function %s, file %s, line %d.\n", \
        __ANLIBC_STR(expression), __func__, __FILE__, __LINE__)}else
#endif /* NDEBUG-else */

#endif /* Guard word. */
