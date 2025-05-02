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
Purpose: memory utilities.

----------------------
 For developers notes
----------------------

*/

#ifndef TDK_MEMUTL_H
#define TDK_MEMUTL_H


#include "tdkbasedefs.h"
#include <memory>

template <typename T>
struct tdk_is_static_creatable : public std::false_type
{

};

// construct
template<typename NoThrowForwardIt, typename Size, typename T>
constexpr NoThrowForwardIt tdk_uninitialized_fill_n(NoThrowForwardIt itFirst,
	Size nCount, const T& value) noexcept
{
	using DestinationValueType = typename std::iterator_traits<NoThrowForwardIt>::value_type;
	NoThrowForwardIt itCurrent = itFirst;

	for (; nCount > 0; ++itCurrent, (void) --nCount)
	{
		void* pDestMem = static_cast<void*>(std::addressof(*itCurrent));
		::new (pDestMem) DestinationValueType(value);
	}
			
	return itCurrent;
}

// destroy

template<class T>
constexpr void tdk_destroy_at(T* ptr) noexcept
{
	if constexpr (std::is_array_v<T>)
		for (auto& elem : *ptr)
			(tdk_destroy_at)(std::addressof(elem));
	else
		ptr->~T();
}

template<typename ForwardIt>
constexpr
void tdk_destroy(ForwardIt first, ForwardIt last) noexcept
{
	for (; first != last; ++first)
		tdk_destroy_at(std::addressof(*first));
}

// copy
template<typename InputIt, typename Size, typename NoThrowForwardIt>
constexpr NoThrowForwardIt tdk_uninitialized_copy_n(
	InputIt itSrcFirst, Size nSrcCount,
	NoThrowForwardIt itDstFirst) noexcept
{
	using DestinationValueType = typename std::iterator_traits<NoThrowForwardIt>::value_type;
	NoThrowForwardIt itDstCurrent = itDstFirst;

	for (; nSrcCount > 0; --nSrcCount, (void)++itSrcFirst, (void) ++itDstCurrent)
	{
		void* pDstMem = static_cast<void*>(std::addressof(*itDstCurrent));
		::new (pDstMem) DestinationValueType(*itSrcFirst);
	}
	
	return itDstCurrent;
}

template<typename InputIt, typename OutputIt>
OutputIt tdk_copy(InputIt itSrcFirst, InputIt itSrcLast, OutputIt itDstFirst) noexcept
{
	for (; itSrcFirst != itSrcLast; (void)++itSrcFirst, (void)++itDstFirst)
		*itDstFirst = *itSrcFirst;

	return itDstFirst;
}

template<typename BidirIt, typename Size, typename NoThrowBidirIt>
NoThrowBidirIt tdk_uninitialized_copy_backward_n(BidirIt itSrcLast, Size nSrcCount,
	NoThrowBidirIt itDstLast) noexcept
{
	using DestinationValueType = typename std::iterator_traits<NoThrowBidirIt>::value_type;
	NoThrowBidirIt itDstCurrent = itDstLast;
	
	for (; nSrcCount > 0; --nSrcCount)
	{
		void* pDstMem = static_cast<void*>(std::addressof(*(--itDstCurrent)));
		::new (pDstMem) DestinationValueType(*(--itSrcLast));
	}

	return itDstCurrent;
}

#endif //TDK_MEMUTL_H
