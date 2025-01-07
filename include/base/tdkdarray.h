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


#include "tdkbasedefs.h"
#include "tdkmemalloc.h"

template <typename TElem, typename TMemAlloc, typename TUtil>
class tdk_darray
{
public:
	typedef tdk_size size_type;
	typedef TElem* iterator;

	

	tdk_darray()
		: m_pData(TDK_NULL)
		, m_nCount(0)
		, m_nCapacity(0)
	{

	}

	~tdk_darray()
	{
		clear();
		resize_memory(0);
	}

	void clear()
	{
		for (size_type i = 0; i < m_nCount; ++i)
		{
			TUtil::destroy(&m_pData[i]);
		}
		m_nCount = 0;
	}

	tdk_u32 grow(size_type nGrowBy = 1)
	{
		tdk_u32 retVal = grow_memory(nGrowBy);
		if (kTDK_OK != retVal)
		{
			return retVal;
		}

		size_type nNewCount = m_nCount + nGrowBy;
		for (size_type i = m_nCount; i < nNewCount; ++i)
		{
			TUtil::construct(m_pData + i);
		}
		m_nCount = nNewCount;
		return kTDK_OK;
	}

	tdk_u32 push_back(const TElem& val)
	{
		tdk_u32 retVal = grow_memory(1);
		if (kTDK_OK != retVal)
		{
			return retVal;
		}
		TUtil::copy_construct(m_pData + m_nCount, val);
		++m_nCount;
		return kTDK_OK;
	}

	tdk_u32 reserve(size_type nNewCap)
	{
		if (nNewCap <= m_nCapacity)
			return kTDK_OK;
		return resize_memory(nNewCap);
	} 

	template<typename EqPred>
	size_type find(const TElem& val, const EqPred& pred,
		size_type startIdx = 0, size_type endIdx = m_nCount) const
	{
		if (endIdx > m_nCount)
			endIdx = m_nCount;

		for (size_type idx = startIdx; idx < endIdx; ++idx)
		{
			if (pred(m_pData[idx], val))
				return idx;
		}
		return m_nCount;
	}

	TElem* at(size_type idx)
	{
		if (idx >= m_nCount)
			return TDK_NULL;
		return m_pData + idx;
	}

	const TElem* at(size_type idx) const
	{
		if (idx >= m_nCount)
			return TDK_NULL;
		return m_pData + idx;
	}

	iterator begin()
	{
		return m_pData;
	}

	iterator end()
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
private:
	tdk_u32 grow_memory(size_type nGrowBy)
	{
		size_type nNewCount = m_nCount + nGrowBy;

		if (nNewCount > m_nCapacity)
		{
			size_type nNewCap = TUtil::grow_capacity(nNewCount, m_nCapacity);
			
			tdk_u32 retVal = resize_memory(nNewCap);
			if (kTDK_OK != retVal)
			{
				return retVal;
			}
		}

		return kTDK_OK;
	}

	tdk_u32 resize_memory(size_type nNewCap)
	{
		tdk_imemalloc* pMemAlloc = TMemAlloc::instance();
		//TDK_ASSERT(pMemAlloc);
    
		size_type nReqBytes = sizeof(TElem) * nNewCap;
		void* pMem = TDK_NULL;
		if (m_pData)
		{
			if (0 == nNewCap)
			{
				pMemAlloc->deallocate(m_pData);
				m_pData = TDK_NULL;
				m_nCapacity = 0;
				return kTDK_OK;
			}

			pMem = pMemAlloc->allocate(nReqBytes);
			if (!pMem)
			{
				return kTDK_FATAL;
			}

			TElem* pMemAsData = (TElem*)pMem;
			for (size_type i = 0; i < m_nCount; ++i)
			{
				TUtil::copy_construct(pMemAsData + i, m_pData[i]);
				TUtil::destroy(&m_pData[i]);
			}

			pMemAlloc->deallocate(m_pData);
			m_pData = pMemAsData;
			m_nCapacity = nNewCap;
			return kTDK_OK;
		}

		if (nNewCap)
		{
			pMem = pMemAlloc->allocate(nReqBytes);
			if (!pMem)
			{
				return kTDK_FATAL;
			}

			m_pData = (TElem*)pMem;
			m_nCapacity = nNewCap;
			return kTDK_OK;
		}
		return kTDK_OK;
	}
	TElem* m_pData;
	size_type m_nCount;
	size_type m_nCapacity;
};
#endif //TDK_DARRAY_H
