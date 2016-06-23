/*************************************************************************
* Ilargia Engine - http://github.com/Xipiryon/Ilargia
* C++ Modular Data Oriented Game Enginee
*------------------------------------------------------------------------
* Copyright (c) 2014-2015, Louis Schnellbach
*
* This software is provided 'as-is', without any express or implied
* warranty. In no event will the authors be held liable for any damages
* arising from the use of this software.
*
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
*
* 1. The origin of this software must not be misrepresented; you must not
*    claim that you wrote the original software. If you use this software
*    in a product, an acknowledgment in the product documentation would
*    be appreciated but is not required.
*
* 2. Altered source versions must be plainly marked as such, and must not
*    be misrepresented as being the original software.
*
* 3. This notice may not be removed or altered from any source
*    distribution.
*
*************************************************************************/

#ifndef INCLUDE_ILARGIA_COMPONENTSTORAGE_HPP
#define INCLUDE_ILARGIA_COMPONENTSTORAGE_HPP

#include <cstdlib>
#include <cstring>
#include <unordered_map>
#include <Muon/System/Assert.hpp>
#include "Ilargia/Core/Define.hpp"

namespace ilg
{
	/*!
	* @brief Array used for storing Component
	* This array has a special storage system primarly
	* designed to work for Component.
	* It is based on an Index identifier: when adding, an index is returned.
	* This is the location in the array where the data is stored.
	* @template T Component you want to store
	* @template chunk How the Array will grow when reaching its memory limit
	*/
	template<typename T, m::i32 ChunkSize>
	class ComponentStorage
	{
	public:
		//----------------------
		// Constructor
		//----------------------
		ComponentStorage()
			: m_capacity(ChunkSize)
			, m_size(0)
		{
			MUON_ASSERT_BREAK(ChunkSize > 0, "Creating 0 Chunk-size!");

			m_buffer = (T*)malloc(sizeof(T) * ChunkSize);
			MUON_ASSERT_BREAK(m_buffer != NULL
							  , "Buffer can't be allocated (Size: %u)"
							  , sizeof(T) * ChunkSize);
		}

		//----------------------
		// Constructor
		//----------------------
		~ComponentStorage()
		{
			free(m_buffer);
			m_capacity = 0;
			m_size = 0;
		}

		//----------------------
		//
		//----------------------
		m::i32 add()
		{
			reallocBuffer();
			m_index[m_size] = m_buffer + m_size;
			new (m_buffer + m_size) T();
			return m_size++;
		}

		m::i32 add(const T& defaultValue)
		{
			reallocBuffer();
			m_index[m_size] = m_buffer + m_size;
			new (m_buffer + m_size) T(defaultValue);
			return m_size++;
		}

		template<typename ...Args>
		m::i32 add(Args...args)
		{
			reallocBuffer();
			m_index[m_size] = m_buffer + m_size;
			new (m_buffer + m_size) T(std::forward<Args>(args)...);
			return m_size++;
		}

		//----------------------
		//
		//----------------------
		bool remove(m::i32 id)
		{
			auto it = m_index.find(id);
			MUON_ASSERT(it != m_index.end(), "Index doesn't exists in Array!");
			if (it == m_index.end())
			{
				return false;
			}

			//Swap last and id, and update index map
			//Only if there is at least two elements
			m_buffer[id].~T();
			m_index.erase(it);
			if (m_size > 1 && ((m_buffer + id) != (m_buffer + m_size - 1)))
			{
				memcpy(m_buffer + id, m_buffer + m_size - 1, sizeof(T));
				//Index map swap only if there is more than 2 elements
				if (m_index.size() > 1)
				{
					m_index[m_size - 1] = m_buffer + id;
				}
			}
			--m_size;
			return true;
		}

		//----------------------
		//
		//----------------------
		m::i32 clear()
		{
			m::i32 nbElement = m_size;
			for (auto it = m_index.begin(); it != m_index.end(); ++it)
			{
				it->second->~T();
			}

			free(m_buffer);
			m_buffer = NULL;
			m_capacity = 0;
			m_size = 0;
			return nbElement;
		}

		//----------------------
		// Getters
		//----------------------
		MUON_INLINE T& get(m::i32 id) const
		{
			return *(m_index.at(id));
		}

		MUON_INLINE T& operator[](m::i32 id) const
		{
			return get(id);
		}

		MUON_INLINE m::i32 size() const
		{
			return m_size;
		}

		MUON_INLINE m::i32 capacity() const
		{
			return m_capacity;
		}

	private:

		void reallocBuffer()
		{
			if (m_size >= m_capacity)
			{
				m_capacity += ChunkSize;
				m::i32 newCapacity = sizeof(T) * m_capacity;
				T* tmpbuff = (T*)realloc(m_buffer, newCapacity);
				MUON_ASSERT_BREAK(tmpbuff != NULL
								  , "Couldn't reallocate new buffer of size: %u (Old capacity: %u | Chunk: %u)"
								  , newCapacity, m_capacity, ChunkSize);
				m_buffer = tmpbuff;
			}
		}

		m::i32 m_capacity;
		m::i32 m_size;
		std::unordered_map<m::i32, T*> m_index;
		T* m_buffer;
	};
}

#endif
