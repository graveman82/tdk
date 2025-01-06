/*
-----------------
 Persistent info
-----------------
 Copyright (C) 2012-2025 Trash Team and graveman
 This file is part of the "Trash Team Development Kit" project.

....................................................
License (is in the "TdkLicense.txt" file and below):
....................................................

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the “Software”), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is furnished
to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

......
 Web:
......

 + https://gamedev.ru/community/trash_team/ (for questions and help)

-------------
 Description
-------------
Purpose: compiler-specific macro definitions.

----------------------
 For developers notes
----------------------

*/

#ifndef TDK_BASEDEFS_H
#define TDK_BASEDEFS_H


//-----------------------------------------------------------------------------
// Compilers family detection

#if defined(_MSC_VER) && !defined(TDK_MSVC)
// Microsoft Visual C++ compiler
#	define TDK_MSVC _MSC_VER

#elif defined(__GNUC__) && !defined(TDK_GNUC)
#	define TDK_GNUC __GNUC__// GNU C compilers
#	define TDK_GNUC_VER \
(__GNUC__ * 10000  + __GNUC_MINOR__ * 100  + __GNUC_PATCHLEVEL__)
...
/* Test for GCC > 3.2.0 */
//#if TDK_GNUC_VER > 30200

#else
#	error "No supported compiler was found."

#endif // compiler

// Integers
#ifdef TDK_USE_INTS_FROM_STDINT_H
#include <stdint.h>

typedef int8_t tdk_s8;
typedef int16_t tdk_s16;
typedef int32_t tdk_s32;

typedef uint8_t tdk_u8;
typedef uint16_t tdk_u16;
typedef uint32_t tdk_u32;
#else
typedef signed char tdk_s8;
typedef short tdk_s16;
typedef int tdk_s32;

typedef unsigned char tdk_u8;
typedef unsigned short tdk_u16;
typedef unsigned tdk_u32;
#endif // integers

typedef tdk_u8 tdk_byte;
typedef tdk_u32 tdk_size;

#define TDK_NULL nullptr

enum tdk_error_codes
{
	kTDK_NO = 0, // fail, no, false, not found
	kTDK_OK = 1, // success, true, yes, found
	kTDK_ERR = 2, // unknown error or just error witout details
	kTDK_CANCEL = 3, // operation was canceled
	kTDK_FATAL = 4, // prrogram termination needed
};
#endif //TDK_BASEDEFS_H
