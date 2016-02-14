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

#ifndef _ILARGIA_ENTITY_H_INCLUDED
#define _ILARGIA_ENTITY_H_INCLUDED

#include <Muon/Helper/NonCopyable.hpp>
#include "Ilargia/Component/Component.hpp"
#include "Ilargia/Component/Array.hpp"

namespace ilg
{
	class EntityManager;
	class ILG_API Entity : public muon::helper::NonCopyable
	{
	public:
		~Entity();

		template<typename T>
		Component addComponent()
		{
			return _addComponent(T::id());
		}

		template<typename T>
		Component getComponent()
		{
			return _getComponent(T::id());
		}

		template<typename T>
		bool removeComponent()
		{
			return _removeComponent(T::id());
		}

	private:
		friend class EntityManager;
		Entity();

		Array<Component, 8>* _components;

		Component _addComponent(muon::i32);
		Component _getComponent(muon::i32);
		bool _removeComponent(muon::i32);
	};
}

#endif
