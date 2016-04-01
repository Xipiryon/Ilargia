/*************************************************************************
* Ilargia Engine - http://github.com/Xleek/Ilargia
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

#ifndef INCLUDE_ILARGIA_COMPONENT_HPP
#define INCLUDE_ILARGIA_COMPONENT_HPP

#include <Muon/Helper/NonCopyable.hpp>
#include <Muon/Meta/MetaDatabase.hpp>
#include <Muon/String.hpp>
#include "Ilargia/Core/Define.hpp"

namespace ilg
{
	namespace manager
	{
		class IBaseManager;
	}
	class Entity;
	class ILARGIA_API Component
	{
	public:
		Component();
		Component(const Component& c);
		Component& operator=(const Component& c);

		m::i32 instanceId() const;
		m::u64 instanceType() const;
		m::String instanceName() const;

		template<typename T>
		MUON_INLINE T* cast() const
		{
			if (T* ptr = (T*)_cast(MUON_META(T)->id(), MUON_META(T)->name()))
			{
				return ptr;
			}
			return NULL;
		}

		template<typename T>
		MUON_INLINE operator T*() const
		{
			return cast<T>();
		}

	private:
		friend class manager::IBaseManager;
		friend class Entity;

		Component(m::u64, m::i32, const m::String&);
		void* _cast(m::u64, const char*) const;

		m::u64 m_type;
		m::i32 m_instance;
		m::String m_name;
	};
}
MUON_TRAITS_DECL(ilg::Component);

#endif
