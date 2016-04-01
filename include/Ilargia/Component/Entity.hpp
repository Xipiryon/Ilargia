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

#ifndef INCLUDE_ILARGIA_ENTITY_HPP
#define INCLUDE_ILARGIA_ENTITY_HPP

#include <deque>
#include <Muon/Helper/NonCopyable.hpp>
#include <Muon/Helper/Singleton.hpp>
#include "Ilargia/Component/Component.hpp"
#include "Ilargia/Component/ComponentStorage.hpp"

namespace ilg
{
	class EntityManager;
	class ILARGIA_API Entity : public m::helper::NonCopyable
	{
		friend class EntityManager;
	public:
		~Entity();

		static Entity* create();
		static void destroy(Entity* entity);

		void setParent(Entity* parent);
		void addChild(Entity* child);

		void removeParent();
		void removeChild(Entity* child);

		template<typename T>
		Component addComponent()
		{
			return _addComponent(MUON_TRAITS(T)::id());
		}

		template<typename T>
		Component getComponent()
		{
			return _getComponent(MUON_TRAITS(T)::id());
		}

		template<typename T>
		bool removeComponent()
		{
			return _removeComponent(MUON_TRAITS(T)::id());
		}

	private:
		Entity();

		ComponentStorage<Component, 8>* m_components;

		Entity* m_parent;
		std::deque<Entity*>* m_children;

		Component _addComponent(m::u64);
		Component _getComponent(m::u64);
		bool _removeComponent(m::u64);
	};

	class ILARGIA_API EntityManager : public m::helper::NonCopyable
	{
		friend class Entity;
	public:
		MUON_SINGLETON_GET(EntityManager);

		Entity* create();
		void destroy(Entity* e);

	private:
		EntityManager();
		~EntityManager();

		void dispatchEntityHierarchyChange(Entity*, Entity*, Entity*);
		std::deque<Entity*>* m_entities;
	};
}

#endif
