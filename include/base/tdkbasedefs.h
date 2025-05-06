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
#	define TDK_MSVC_VER _MSC_VER

#elif defined(__GNUC__) && !defined(TDK_GNUC)
#	define TDK_GNUC_VER \
	(__GNUC__ * 10000  + __GNUC_MINOR__ * 100  + __GNUC_PATCHLEVEL__)

/* Test for GCC > 3.2.0 */
//#if TDK_GNUC_VER > 30200

#else
#	error "No supported compiler was found."

#endif // compiler

//-----------------------------------------------------------------------------
// C++ standard detection

#if __cplusplus >= 202002L
#define TDK_CPP20
#define TDK_CPP17
#define TDK_CPP17
#define TDK_CPP14
#define TDK_CPP11
#define TDK_CPP03
#endif

#if __cplusplus >= 201703L
#define TDK_CPP17
#define TDK_CPP14
#define TDK_CPP11
#define TDK_CPP03
#endif

// Integers
#include <cstdint>
#include <cstddef>

using tdk_byte = std::uint8_t;
using tdk_size = std::size_t;
using tdk_diff = std::ptrdiff_t;
using tdk_u32 = std::uint32_t;
using tdk_u16 = std::uint16_t;

// Reals
using tdk_real32 = float;
using tdk_real64 = double;

const tdk_real32 kTDK_REAL32_ZERO = 0.0f;
const tdk_real32 kTDK_REAL32_ONE = 1.0f;
const tdk_real32 kTDK_REAL32_HALF = 0.5f;
const tdk_real32 kTDK_REAL32_QUARTER = 0.25f;

// Errors
enum tdk_operation_codes
{
	kTDK_NO = 0, // fail, no, false, not found
	kTDK_OK = 1, // success, true, yes, found
	kTDK_ERR = 2, // unknown error or just error witout details
	kTDK_CANCEL = 3, // operation was canceled
	kTDK_FATAL = 4, // prrogram termination needed
};

using tdk_ret = tdk_u32;
using tdk_err = tdk_u32;

enum tdk_error_codes
{
	kTDK_BAD_ALLOC,
	kTDK_BAD_SIZE,
};

#define TDK_UNUSED(x) ((void)x)
#endif //TDK_BASEDEFS_H
