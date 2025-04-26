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
Purpose: dynamic array.

----------------------
 For developers notes
----------------------

*/

#ifndef TDK_DARRAY_H
#define TDK_DARRAY_H


#include "base/tdkmemalloc.h"
#include "base/tdkmemutl.h"

#include <memory>
#include <cassert>

template <typename T, typename Allocator = tdk_allocator<T>>
class tdk_darray
{
	using AllocatorForT = typename std::allocator_traits<Allocator>::template rebind_alloc<T>;
	using AllocatorTraitsForT = std::allocator_traits<AllocatorForT>;
public:
	using size_type = tdk_size;
	using difference_type = tdk_diff;

	using value_type = T;
	using allocator_type = Allocator;
		
	using reference = value_type&;
	using const_reference = const value_type&;
	using pointer = typename std::allocator_traits<Allocator>::pointer;
	using const_pointer = typename std::allocator_traits<Allocator>::const_pointer;
	using iterator = pointer;
	using const_iterator = const_pointer;
	using reverse_iterator = std::reverse_iterator<iterator>;
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;

	tdk_darray() = default;

	~tdk_darray()
	{
		resize_memory(0);
	}

	void clear()
	{
		destroy_all(0);
	}

	tdk_ret grow(size_type nGrowBy = 1, tdk_err* pErrorCode = nullptr)
	{
		tdk_ret retVal = grow_memory(nGrowBy, pErrorCode);
		if (retVal != kTDK_OK)
		{
			return retVal;
		}

		tdk_uninitialized_fill_n(m_pData + m_nCount, nGrowBy, T());
		m_nCount = m_nCount + nGrowBy;
		return kTDK_OK;
	}

	tdk_ret push_back(const T& val, tdk_err* pErrorCode = nullptr)
	{
		tdk_ret retVal = grow_memory(1, pErrorCode);
		if (retVal != kTDK_OK)
		{
			return retVal;
		}
		tdk_uninitialized_copy_n(std::addressof(val), 1, m_pData + m_nCount);
		++m_nCount;
		return kTDK_OK;
	}

	tdk_ret reserve(size_type nNewCap, tdk_err* pErrorCode = nullptr)
	{
		if (nNewCap <= m_nCapacity)
			return kTDK_OK;
		return resize_memory(nNewCap, pErrorCode);
	} 

	T* at(size_type idx)
	{
		if (idx >= m_nCount)
			return nullptr;
		return m_pData + idx;
	}

	const T* at(size_type idx) const
	{
		if (idx >= m_nCount)
			return nullptr;
		return m_pData + idx;
	}

	iterator begin()
	{
		return m_pData;
	}

	const_iterator begin() const
	{
		return m_pData;
	}

	iterator end()
	{
		return m_pData + m_nCount;
	}

	const_iterator end() const
	{
		return m_pData + m_nCount;
	}

	size_type size() const
	{
		return m_nCount;
	}

	size_type capacity() const
	{
		return m_nCapacity;
	}

	allocator_type get_allocator() const
	{
		return allocator(*get_allocator_for_T());
	}
private:

	template<typename A>
	std::enable_if_t<tdk_is_static_creatable<A>::value, bool>
	is_allocator_static_creatable(A* pUnused)
	{
		TDK_UNUSED(pUnused);
		return true;
	}

	bool is_allocator_static_creatable(...)
	{
		return false;
	}

	template<typename A>
	std::enable_if_t<tdk_is_static_creatable<A>::value, A*>
	create_allocator_on_static_memory(void* pAllocatorMemory, A* pUnused)
	{
		TDK_UNUSED(pUnused);
		return ::new(pAllocatorMemory) A();
	}

	AllocatorForT* create_allocator_on_static_memory(...)
	{
		return nullptr;
	}

	static void* get_allocator_static_memory()
	{
		static tdk_byte mem[sizeof(AllocatorForT)];
		return mem;
	}

	AllocatorForT* get_allocator_for_T()
	{
		AllocatorForT* pResult{};
		if (is_allocator_static_creatable(pResult))
		{
			pResult = create_allocator_on_static_memory(
				get_allocator_static_memory(),
				pResult);
		}
		
		assert(pResult);
		return pResult;
	}
		
	size_type suggest_capacity(size_type nNewCount, size_type nCurrentCap)
	{
		nCurrentCap = tdk_max(nCurrentCap, size_type(4));

		while (nCurrentCap < nNewCount)
		{
			nCurrentCap += nCurrentCap >> 1;
		}

		return nCurrentCap;
	}

	tdk_ret grow_memory(size_type nGrowBy, tdk_err* pErrorCode)
	{
		size_type nNewCount = m_nCount + nGrowBy;

		if (nNewCount > m_nCapacity)
		{
			size_type nNewCap = suggest_capacity(nNewCount, m_nCapacity);
			
			tdk_ret retVal = resize_memory(nNewCap, pErrorCode);
			if (retVal != kTDK_OK)
			{
				return retVal;
			}
		}

		return kTDK_OK;
	}

	tdk_ret resize_memory(size_type nNewCap, tdk_err* pErrorCode = nullptr)
	{
		AllocatorForT* pAllocatorForT = get_allocator_for_T();
		assert(pAllocatorForT);
    
		if (0 == nNewCap)
		{
			return free_memory(pErrorCode);
		}

		T* pNewData = pAllocatorForT->allocate(nNewCap);
		if (!pNewData)
		{
			tdk_set_error_code(pErrorCode, kTDK_BAD_ALLOC);
			return kTDK_FATAL;
		}

		if (m_nCount)
		{
			assert(m_pData);
			tdk_uninitialized_copy_n(m_pData, m_nCount, pNewData);
			tdk_destroy(m_pData, m_pData + m_nCount);
		}

		if (m_pData)
		{
			pAllocatorForT->deallocate(m_pData, m_nCapacity);
		}

		m_pData = pNewData;
		m_nCapacity = nNewCap;
		return kTDK_OK;
	}

	tdk_ret free_memory(tdk_err* pErrorCode = nullptr)
	{
		AllocatorForT* pAllocatorForT = get_allocator_for_T();
		assert(pAllocatorForT);
		
		destroy_all(pErrorCode);

		if (m_pData)
		{
			pAllocatorForT->deallocate(m_pData, m_nCapacity);
			m_pData = nullptr;
			m_nCapacity = 0;
		}
		
		return kTDK_OK;
	}

	tdk_ret destroy_all(tdk_err* pErrorCode = nullptr)
	{
		if (m_nCount)
		{
			assert(m_pData);
			tdk_destroy(m_pData, m_pData + m_nCount);
			m_nCount = 0;
		}

		return kTDK_OK;
	}

	T* m_pData{};
	size_type m_nCount{};
	size_type m_nCapacity{};
};
#endif //TDK_DARRAY_H
