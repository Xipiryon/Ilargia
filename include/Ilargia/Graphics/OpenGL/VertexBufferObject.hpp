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

#ifndef INCLUDE_ILARGIA_VBO_HPP
#define INCLUDE_ILARGIA_VBO_HPP

#include <Muon/System/Assert.hpp>
#include <gl/gl.h>

namespace ilg
{
	namespace graphics
	{
		template <typename T>
		class VertexBufferObject
		{
		public:
			VertexBufferObject()
				: m_array(NULL)
				, m_mappedArray(NULL)
				, m_size(0)
				, m_buffer(NULL)
				, m_target(GL_ARRAY_BUFFER)
				, m_isMapped(false)
			{
			}

			VertexBufferObject(m::u32 size)
				: VertexBufferObject()
			{
				initBuffer(size);
			}

			~VertexBufferObject()
			{
				deleteBuffer();
			}

			T& operator[](m::u32 index)
			{
				MUON_ASSERT_BREAK(index >= 0 && index < m_size, "Outside boundary acces!");
				return m_array[index];
			}

			void initBuffer(m::u32 size)
			{
				clear();
				glGenBuffers(1, &m_buffer);
				m_array = (T*)::malloc(sizeof(T) * size);
				m_size = size;
			}

			void deleteBuffer()
			{
				if (m_size > 0)
				{
					glDeleteBuffers(1, &m_buffer);
					free(m_array);
					m_array = NULL;
					m_size = 0;
				}
			}

			void bindBuffer(GLenum target = GL_ARRAY_BUFFER)
			{
				glBindBuffer(target, m_buffer);
				m_target = target;
			}

			void setupBuffer(GLenum type = GL_STATIC_DRAW)
			{
				glBufferData(m_target, dataSize(), m_array, type);
			}

			T* mapBuffer(GLenum accessType)
			{
				if (!m_isMapped)
				{
					m_isMapped = true;
					bindBuffer(m_target);
					m_mappedArray = reinterpret_cast<T*>(glMapBuffer(m_target, accessType));
				}
				return m_mappedArray;
			}

			bool unmapBuffer()
			{
				m_isMapped = false;
				glBindBuffer(m_target, m_buffer);
				return glUnmapBuffer(m_target);
			}

			T* getArray() const
			{
				return m_array;
			}

			T* setArray(T* data, m::u32 size)
			{
				::memcpy(m_array, data, m_size * sizeof(T));
				return m_array;
			}

			const m::u32 getSize() const
			{
				return m_size;
			}

			const m::u32 getDataSize() const
			{
				return m_size * sizeof(T);
			}

		private:
			T* m_array;
			T* m_mappedArray;
			m::u32 m_size;
			m::u32 m_buffer;
			GLenum m_target;
			bool m_isMapped;
		};
	}
}

#endif
