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
of this software and associated documentation files (the �Software�), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is furnished
to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED �AS IS�, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
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

#ifndef TDK_MEMALLOC_H
#define TDK_MEMALLOC_H

#include "system/tdkmemory.h"
#include "base/tdkmemutl.h"

template<typename T, tdk_size kAlign = 16>
class tdk_allocator
{
public:
    using size_type = tdk_size;
    using difference_type = tdk_diff;

    using value_type = T;

    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = T*;
    using const_pointer = const T*;

    template<typename U>
    struct rebind
    {
        using other = tdk_allocator<U, kAlign>;
    };

  

    tdk_allocator() noexcept = default;

    tdk_allocator(const tdk_allocator& oth) noexcept = default;

    tdk_allocator& operator=(const tdk_allocator&) noexcept = default;

    template<typename U>
    tdk_allocator(const tdk_allocator<U>&) noexcept { }

    ~tdk_allocator() noexcept { }
  
    

    T* allocate(size_type n)
    {
        return static_cast<T*>(tdk_allocate_memory_aligned(n * sizeof(T), kAlign));
    }

    void deallocate(T* p, size_type n)
    {
        TDK_UNUSED(n);
        tdk_free_memory_aligned(p, kAlign);
    }
};



template <typename T, tdk_size kAlign>
struct tdk_is_static_creatable<tdk_allocator<T, kAlign>> : public std::true_type
{

};
#endif //TDK_MEMALLOC_H
