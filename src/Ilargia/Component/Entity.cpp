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

#include <Muon/System/Assert.hpp>
#include <Muon/Memory/Allocator.hpp>
#include "Ilargia/Manager/ManagerFactory.hpp"
#include "Ilargia/Component/Entity.hpp"

namespace ilg
{
	Entity::Entity()
		: m_parent(NULL)
	{
		typedef ComponentStorage<Component, 8> ComponentArray;
		m_components = MUON_NEW(ComponentArray);
		m_children = MUON_NEW(std::deque<Entity*>);
	}

	Entity::~Entity()
	{
		MUON_DELETE(m_components);
		MUON_DELETE(m_children);
	}

	Entity* Entity::create()
	{
		return EntityManager::getInstance().create();
	}

	void Entity::destroy(Entity* entity)
	{
		EntityManager::getInstance().destroy(entity);
	}

	void Entity::setParent(Entity* parent)
	{
		// If already my parent, skip
		if (m_parent == parent)
		{
			return;
		}

		// Update current parent, remove me from its children
		Entity* oldParent = m_parent;
		if (m_parent != NULL)
		{
			for (auto it = m_parent->m_children->begin(); it != m_parent->m_children->end(); ++it)
			{
				if (*it == this)
				{
					m_parent->m_children->erase(it);
					break;
				}
			}
		}

		// Add me in new parent children
		if (parent != NULL)
		{
			parent->m_children->push_back(this);
		}

		// Update and notify
		m_parent = parent;
		EntityManager::getInstance().dispatchEntityHierarchyChange(this, oldParent, parent);
	}

	void Entity::addChild(Entity* child)
	{
		if (child != NULL)
		{
			// Case the child we want is our parent
			if (m_parent == child)
			{
				m_parent = NULL;
				EntityManager::getInstance().dispatchEntityHierarchyChange(this, child, m_parent);
			}

			// Remove the child from its parent
			Entity* oldParent = child->m_parent;
			if (child->m_parent)
			{
				if (child->m_parent == this)
				{
					return;
				}
				child->m_parent->removeChild(child);
			}

			// Update and notify
			child->m_parent = this;
			m_children->push_back(child);
			EntityManager::getInstance().dispatchEntityHierarchyChange(child, oldParent, this);
		}
	}

	void Entity::removeParent()
	{
		if (m_parent != NULL)
		{
			// Search for myself in parent children and remove me
			Entity* oldParent = m_parent;
			for (auto it = m_parent->m_children->begin(); it != m_parent->m_children->end(); ++it)
			{
				if (*it == this)
				{
					m_parent->m_children->erase(it);
					break;
				}
			}

			// Update and notify
			m_parent = NULL;
			EntityManager::getInstance().dispatchEntityHierarchyChange(this, oldParent, m_parent);
		}
	}

	void Entity::removeChild(Entity* child)
	{
		if (child->m_parent == this)
		{
			// Iterate and remove it
			for (auto it = m_children->begin(); it != m_children->end(); ++it)
			{
				if (*it == child)
				{
					m_children->erase(it);
					break;
				}
			}

			// Update and notify
			child->m_parent = NULL;
			EntityManager::getInstance().dispatchEntityHierarchyChange(child, this, child->m_parent);
		}
	}

	Component Entity::_addComponent(m::u64 type)
	{
		manager::IBaseManager* manager = ILARGIA_GET_COMPONENT_MANAGER_FROM_TYPE(manager::IBaseManager, type);
		Component c;
		if (manager)
		{
			c = manager->createComponent();
			m_components->add(c);
			manager->onComponentAdded(this, c);
		}
		return c;
	}

	Component Entity::_getComponent(m::u64 type)
	{
		for (m::i32 i = 0; i < m_components->size(); ++i)
		{
			Component c = m_components->get(i);
			if (c.getInstanceTypeId() == type)
			{
				return c;
			}
		}
		return Component();
	}

	bool Entity::_removeComponent(m::u64 type)
	{
		manager::IBaseManager* manager = ILARGIA_GET_COMPONENT_MANAGER_FROM_TYPE(manager::IBaseManager, type);
		Component c;
		if (manager)
		{
			for (m::i32 i = 0; i < m_components->size(); ++i)
			{
				Component c = m_components->get(i);
				if (c.getInstanceTypeId() == type)
				{
					manager->onComponentRemoved(this, c);
					manager->destroyComponent(c);
					m_components->remove(i);
					return true;
				}
			}
		}
		return false;
	}

	EntityManager::EntityManager()
	{
		typedef std::deque<Entity*> EntityDeque;
		m_entities = MUON_NEW(EntityDeque);
	}

	EntityManager::~EntityManager()
	{
		MUON_DELETE(m_entities);
	}

	Entity* EntityManager::create()
	{
		Entity* e = MUON_NEW(Entity);
		m_entities->push_back(e);
		return e;
	}

	void EntityManager::destroy(Entity* e)
	{
		for (auto it = m_entities->begin(); it != m_entities->end(); ++it)
		{
			if ((*it) == e)
			{
				m_entities->erase(it);
				break;
			}
		}
		MUON_DELETE(e);
	}

	void EntityManager::dispatchEntityHierarchyChange(Entity* entity, Entity* oldParent, Entity* newParent)
	{
		auto& managerFactory = manager::ManagerFactory::getInstance();
		for (m::u32 i = 0; i < managerFactory.getComponentManagerCount(); ++i)
		{
			managerFactory.getComponentManagerFromIndex(i)->onEntityHierarchyChanged(entity, oldParent, newParent);
		}
	}
}
