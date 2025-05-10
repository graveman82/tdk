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

#ifndef TDK_MEMORYPOOL_H_INCLUDED
#define TDK_MEMORYPOOL_H_INCLUDED

#include "base/tdkbasedefs.h"
#include "system/tdkmemory.h"

#define TDK_MEMORY_POOL_DEBUG_MODE 0

//-----------------------------------------------------------------------------
//
template <tdk_size kTypeSize>
class tdk_memorypool
{
public:
	typedef tdk_size size_type;

private:
	class Node;
	typedef Node* NodePtr;

	class Node
	{
	public:
		Node() : m_pNext(0)
		{
#if TDK_MEMORY_POOL_DEBUG_MODE
		    std::cout << "Node[" << uintptr_t(this) << "]: constructor()\n";
		    
#endif
        }
#if TDK_MEMORY_POOL_DEBUG_MODE
		~NodeBase()
		{
		    std::cout << "Node[" << uintptr_t(this) << "]: destructor()\n";
		    
        }
#endif

		NodePtr get_next()
		{
			return m_pNext;
		}

		const NodePtr get_next() const
		{
			return m_pNext;
		}

		void set_next(NodePtr pNext)
		{
			m_pNext = pNext;
		}

		tdk_byte* get_memory()
		{
			return &m_memory[0];
		}

		tdk_diff memory_offset() const
		{
			return ((tdk_byte*)&m_memory[0]) - ((tdk_byte*)this);
		}

	private:
		NodePtr m_pNext;
		tdk_byte m_memory[kTypeSize]; // data are placed there
	};


	struct Block;
	typedef Block* BlockPtr;

	class Block
	{
	public:
		Block()
			: m_pNext(0)
			, m_pNodes(0)
			, m_nCapacity(0)
		{
#if CPK_SINGLETON_DEBUG_MODE
            std::cout << "Block[" << uintptr_t(this) << "]: constructor()\n";
#endif
		}

        ~Block()
        {
#if CPK_SINGLETON_DEBUG_MODE
            std::cout << "Block[" << uintptr_t(this) << "]: destructor()\n";
#endif
        }

		tdk_ret initialize(void* pNodeMem, size_type nCapacity, tdk_err* pErrorCode = 0)
		{
			assert(!m_pNodes);
			m_pNodes = reinterpret_cast<NodePtr>(pNodeMem);
			tdk_uninitialized_fill_n(m_pNodes, nCapacity, Node());
			m_nCapacity = nCapacity;
			return kTDK_OK;
		}

		void set_next(BlockPtr pNext)
		{
			m_pNext = pNext;
		}

		Node* get_nodes()
		{
			return m_pNodes;
		}

		tdk_size capacity() const
		{
			return m_nCapacity;
		}
	private:
		BlockPtr m_pNext;
		NodePtr m_pNodes;
		tdk_size m_nCapacity;
	};


public:
	void *allocate(tdk_err* pErrorCode = 0);
	void free(void *);
    size_type capacity() const { return m_nCapacity; }

    virtual ~tdk_memorypool();
	tdk_memorypool();

private:
	size_type suggest_capacity(size_type nCurrentCap);
	tdk_ret reserve(tdk_err* pErrorCode = 0);
	NodePtr convert(void*);

	BlockPtr m_pFirstBlock;
	NodePtr m_pFirstUnusedNode;
	size_type m_nCapacity;
};


//-----------------------------------------------------------------------------

template <tdk_size kTypeSize>
tdk_memorypool<kTypeSize>::tdk_memorypool()
	: m_pFirstBlock(0)
	, m_pFirstUnusedNode(0)
	, m_nCapacity(0)
{
#if CPK_MEMORY_POOL_DEBUG_MODE
    std::cout << "tdk_memorypool<" << kTypeSize << ">::tdk_memorypool()\n";
    
#endif
}

//-----------------------------------------------------------------------------

template <tdk_size kTypeSize>
tdk_memorypool<kTypeSize>::~tdk_memorypool()
{
#if CPK_MEMORY_POOL_DEBUG_MODE
    std::cout << "tdk_memorypool<" << kTypeSize << ">::~tdk_memorypool()\n";
    
#endif
}

//-----------------------------------------------------------------------------
template <tdk_size kTypeSize>
typename tdk_memorypool<kTypeSize>::size_type
tdk_memorypool<kTypeSize>::suggest_capacity(size_type nCurrentCap)
{
	nCurrentCap = tdk_max(nCurrentCap, size_type(4));
	nCurrentCap += nCurrentCap >> 1;
	return nCurrentCap;
}

//-----------------------------------------------------------------------------

template <tdk_size kTypeSize>
tdk_ret
tdk_memorypool<kTypeSize>::reserve(tdk_err* pErrorCode)
{
	size_type nNewCapacity = suggest_capacity(m_nCapacity);
	void* pMem = tdk_allocate_memory_aligned(sizeof(Block) + sizeof(Node) * nNewCapacity, 16);
	if (!pMem)
	{
		tdk_set_error_code(pErrorCode, kTDK_BAD_ALLOC);
		return kTDK_FATAL;
	}

	BlockPtr pNewBlock = reinterpret_cast<BlockPtr>(pMem);

	tdk_uninitialized_fill_n(pNewBlock, 1, Block());
	pNewBlock->initialize(pNewBlock + 1, nNewCapacity, pErrorCode);

	m_nCapacity += nNewCapacity;
#if CPK_MEMORY_POOL_DEBUG_MODE
    std::cout << "reserve() before block linking\n";
    
#endif
	pNewBlock->set_next(m_pFirstBlock);
	m_pFirstBlock = pNewBlock;
#if CPK_MEMORY_POOL_DEBUG_MODE
    std::cout << "reserve() after block linking\n";
    
#endif
	NodePtr pNewNodes = m_pFirstBlock->get_nodes();
	for(size_type i = 0; i < m_pFirstBlock->capacity() - 1; ++i)
    {
		pNewNodes[i].set_next(&pNewNodes[i + 1]);
	}

	m_pFirstUnusedNode = &pNewNodes[0];
#if CPK_MEMORY_POOL_DEBUG_MODE
    std::cout << "reserve() end of function newBlock.get(): " << uintptr_t(pNewBlock) << "\n";
    
#endif
	return kTDK_OK;
}

//-----------------------------------------------------------------------------

template <tdk_size kTypeSize>
void*
tdk_memorypool<kTypeSize>::allocate(tdk_err* pErrorCode)
{
	if (!m_pFirstUnusedNode) 
	{ 
		tdk_ret retval = reserve(pErrorCode);
		if (retval != kTDK_OK)
			return 0;
	}
	assert(m_pFirstUnusedNode);
	NodePtr pResultNode = m_pFirstUnusedNode;
	m_pFirstUnusedNode = m_pFirstUnusedNode->get_next();
	pResultNode->set_next(0);

	return pResultNode->get_memory();
}

//-----------------------------------------------------------------------------

template <tdk_size kTypeSize>
void
tdk_memorypool<kTypeSize>::free(void* p)
{
	NodePtr pNode = convert(p);

	pNode->set_next(m_pFirstUnusedNode);
	m_pFirstUnusedNode = pNode;
}

//-----------------------------------------------------------------------------

template <tdk_size kTypeSize>
typename tdk_memorypool<kTypeSize>::NodePtr
tdk_memorypool<kTypeSize>::convert(void* p)
{
	Node auxNode;
	tdk_diff nOffset = auxNode.memory_offset();

	tdk_byte* pDataBytes = reinterpret_cast<tdk_byte*>(p);
	NodePtr pResultNode = reinterpret_cast<NodePtr>(pDataBytes - nOffset);

	return pResultNode;
}


#endif // TDK_MEMORYPOOL_H_INCLUDED
