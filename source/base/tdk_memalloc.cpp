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
Purpose: memory allocators.

----------------------
 For developers notes
----------------------

*/

#include "tdkmemalloc.h"
#include <stdlib.h>

namespace tdk
{
	namespace details
	{
		static tdk_memalloc_mf s_memalloc_mf;
	} // eof details
} // eof tdk

tdk_imemalloc* tdk_memalloc_mf::instance()
{
	return &tdk::details::s_memalloc_mf;
}

void* tdk_memalloc_mf::allocate(tdk_u32 nBytes)
{
	return malloc(nBytes);
}

void* tdk_memalloc_mf::reallocate(void* p, tdk_u32 nBytes)
{
	return realloc(p, nBytes);
}

void tdk_memalloc_mf::deallocate(void* p)
{
	free(p);
}