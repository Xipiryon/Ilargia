/*************************************************************************
* DNA Engine - http://gitweb.louis-schnellbach.com
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

#ifndef _ILARGIA_ARRAY_H_INCLUDED
#define _ILARGIA_ARRAY_H_INCLUDED

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
	template<typename T, muon::i32 chunk>
	class Array
	{
	public:
		//----------------------
		// Constructor
		//----------------------
		Array()
			: _capacity(chunk)
			, _chunk(chunk)
			, _size(0)
		{
			MUON_ASSERT_BREAK(chunk > 0, "Creating 0 Chunk-size!");

			_buffer = (T*)malloc(sizeof(T) * _chunk);
			MUON_ASSERT_BREAK(_buffer != NULL
				, "Buffer can't be allocated (Size: %u)"
				, sizeof(T) * _chunk);
		}

		//----------------------
		// Constructor
		//----------------------
		~Array()
		{
			free(_buffer);
			_capacity = 0;
			_size = 0;
		}

		//----------------------
		//
		//----------------------
		muon::i32 add()
		{
			reallocBuffer();
			_index[_size] = _buffer + _size;
			new (_buffer + _size) T();
			return _size++;
		}

		muon::i32 add(const T& defaultValue)
		{
			reallocBuffer();
			_index[_size] = _buffer + _size;
			new (_buffer + _size) T(defaultValue);
			return _size++;
		}

		template<typename ...Args>
		muon::i32 add(Args...args)
		{
			reallocBuffer();
			_index[_size] = _buffer + _size;
			new (_buffer + _size) T(std::forward<Args>(args)...);
			return _size++;
		}

		//----------------------
		//
		//----------------------
		bool remove(muon::i32 id)
		{
			auto it = _index.find(id);
			MUON_ASSERT(it != _index.end(), "Index doesn't exists in Array!");
			if (it == _index.end())
			{
				return false;
			}

			//Swap last and id, and update index map
			//Only if there is at least two elements
			_buffer[id].~T();
			_index.erase(it);
			if (_size > 1 && ((_buffer + id) != (_buffer + _size - 1)))
			{
				memcpy(_buffer + id, _buffer + _size - 1, sizeof(T));
				//Index map swap only if there is more than 2 elements
				if (_index.size() > 1)
				{
					_index[_size - 1] = _buffer + id;
				}
			}
			--_size;
			return true;
		}

		//----------------------
		//
		//----------------------
		muon::i32 clear()
		{
			muon::i32 nbElement = _size;
			for (auto it = _index.begin(); it != _index.end(); ++it)
			{
				it->second.~T();
			}

			free(_buffer);
			_buffer = NULL;
			_capacity = 0;
			_size = 0;
			return nbElement;
		}

		//----------------------
		// Getters
		//----------------------
		MUON_INLINE T& get(muon::i32 id) const
		{
			return *(_index.at(id));
		}

		MUON_INLINE T& operator[](muon::i32 id) const
		{
			return get(id);
		}

		MUON_INLINE muon::i32 size() const
		{
			return _size;
		}

		MUON_INLINE muon::i32 capacity() const
		{
			return _capacity;
		}

	private:

		void reallocBuffer()
		{
			if (_size >= _capacity)
			{
				_capacity += _chunk;
				muon::i32 newCapacity = sizeof(T) * _capacity;
				T* tmpbuff = (T*)realloc(_buffer, newCapacity);
				MUON_ASSERT_BREAK(tmpbuff != NULL
					, "Couldn't reallocate new buffer of size: %u (Old capacity: %u | Chunk: %u)"
					, newCapacity, _capacity, _chunk);
				_buffer = tmpbuff;
			}
		}

		const muon::i32 _chunk;
		muon::i32 _capacity;
		muon::i32 _size;
		std::unordered_map<muon::i32, T*> _index;
		T* _buffer;
	};
}

#endif
