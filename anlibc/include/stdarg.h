/*
    stdarg.h
    Aean, 2014.7.1
    This file is part of anlibc, which itself is part of ancc project.
    ancc project is licensed under the terms of the MIT License.
    Copyright (C) 2014 Aean & HUST
*/

#ifndef __ANLIBC_STDARG_INCLUDED_
#define __ANLIBC_STDARG_INCLUDED_

typedef char* va_list;
#define va_start(vl, arg) if(1){ vl = (va_list)( &(arg) ) + ( (sizeof(arg) + sizeof(int) - 1) & ~(sizeof(int) - 1) ); }else
#define va_end(vl) if(1){ vl = (va_list)0; }else
#define va_arg(vl, type) ( *((type *)((vl += ( (sizeof(type) + sizeof(int) - 1) & ~(sizeof(int) - 1) )) - ( (sizeof(type) + sizeof(int) - 1) & ~(sizeof(int) - 1) ))) )

#endif /* Guard word. */
