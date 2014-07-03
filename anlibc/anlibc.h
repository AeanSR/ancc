/*
    anlibc.h
    Aean, 2014.6.29
    This file is part of anlibc, which itself is part of ancc project.
    ancc project is licensed under the terms of the MIT License.
    Copyright (C) 2014 Aean & HUST
*/

#ifndef __ANLIBC_HEADER_INCLUDED_
#define __ANLIBC_HEADER_INCLUDED_

#define __ANLIBC_INTLBUILD__

#include "include\stddef.h"
#include "include\stdint.h"
#include "include\stdarg.h"
#include "include\limits.h"
#include "include\string.h"
#include "include\stdlib.h"
#include "include\stdio.h"

//void* __stdcall VirtualAlloc(void* addr, unsigned size, unsigned alloctype, unsigned prot);
//int __stdcall VirtualFree(void* addr, unsigned size, unsigned ftype);
void* __stdcall GetStdHandle(unsigned nStdHandle);
int __stdcall ReadFile(void* hFile, void* lpBuffer, unsigned nNumberOfBytesToRead, unsigned* lpNumberOfBytesRead, void* lpOverlapped);
int __stdcall WriteFile(void* hFile, const void* lpBuffer, unsigned nNumberOfBytesToWrite, unsigned* lpNumberOfBytesWritten, void* lpOverlapped);
void* __stdcall CreateFile(const char* lpFileName, unsigned dwDesiredAccess, unsigned dwShareMode, void* lpSecurityAttributes, unsigned dwCreationDisposition, unsigned dwFlagsAndAttributes, void* hTemplateFile);
int __stdcall CloseHandle(void* hObject);
int __stdcall FlushFileBuffers(void* hFile);
void* __stdcall HeapAlloc(void* hHeap, unsigned dwFlags, unsigned dwBytes);
int __stdcall HeapFree(void* hHeap, unsigned dwFlags, void* lpMem);
void* __stdcall HeapReAlloc(void* hHeap, unsigned dwFlags, void* lpMem, unsigned dwBytes);
void* __stdcall GetProcessHeap(void);


#endif /* Guard word. */
