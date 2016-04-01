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

#include <Muon/System/Assert.hpp>

#include "Ilargia/Manager/ManagerFactory.hpp"
#include "Ilargia/Component/Transform.hpp"

namespace ilg
{
	Transform::Transform()
		: position(0.f, 0.f, 0.f)
		, scale(1.f, 1.f, 1.f)
		, rotation(Quaternion::Identity)
		, m_model(Matrix::Identity)
	{
	}

	Transform::~Transform()
	{
	}

	Matrix Transform::getMatrix() const
	{
		return m_model;
	}

	ILARGIA_COMPONENT_MANAGER_NAME(Transform)::ILARGIA_COMPONENT_MANAGER_NAME(Transform)()
		: IComponentManager(160)
		, m_requireRootListUpdate(true)
		, m_rootTransforms(NULL)
	{
	}

	ILARGIA_COMPONENT_MANAGER_NAME(Transform)::~ILARGIA_COMPONENT_MANAGER_NAME(Transform)()
	{
	}

	void ILARGIA_COMPONENT_MANAGER_NAME(Transform)::onInit()
	{
		/*
		uint32_t chunk = 64;
		Global::retrieve<uint32_t>("MEMCHUNK_TRANSFORM", &chunk);
		m_components = new Array<Transform>(chunk);
		m_rootTransforms = new Array<Component>(chunk);
		Log::debug("[TRANSFORM] Initialized (%d)", chunk);
		//*/
	}

	void ILARGIA_COMPONENT_MANAGER_NAME(Transform)::onUpdate(m::f32 dt)
	{
		/*
		updateRootList();
		for (auto it = m_rootTransforms->iterator(); it; ++it)
		{
		//Fast retrieve of the Transform* from the Component
		//(We don't go with it.value()->cast<>()...
		updateRecursive(it.value());
		}
		//*/
	}

	void ILARGIA_COMPONENT_MANAGER_NAME(Transform)::onTerm()
	{
		/*
		if(m_components != NULL)
		{
		for(auto it = m_components->iterator(); it; ++it)
		{
		it.value().~Transform();
		}
		delete m_components;
		m_components = NULL;
		}

		if(m_rootTransforms != NULL)
		{
		delete m_rootTransforms;
		m_rootTransforms = NULL;
		}
		//*/
	}

	void ILARGIA_COMPONENT_MANAGER_NAME(Transform)::onEntityHierarchyChanged(Entity* entity, Entity* previousParent, Entity* newParent)
	{
	}

	void ILARGIA_COMPONENT_MANAGER_NAME(Transform)::updateRootList()
	{
		if (m_requireRootListUpdate)
		{
			/*
			//Recreate the whole list
			m::i32 transform_type = type_id<Transform>();
			m_rootTransforms->clear();
			for (auto it = m_components->iterator(); it; ++it)
			{
			Transform& t = it.value();
			//Only had if the Transform have no valid parent
			if (!t._parent.isValid())
			{
			m_rootTransforms->add(makeComponent(it.index(), transform_type));
			}
			}
			//*/
		}
	}

	void ILARGIA_COMPONENT_MANAGER_NAME(Transform)::updateRecursive(Component& component)
	{
		/*
		Transform* transform = &(*m_components)[component.getID()];
		//Check for a Destroy flag
		if(transform->flags & ObjectType::REQUIRE_DELETE)
		{
		destroy(component);
		rootListChanged();
		}
		else
		{
		//Update itself
		if(transform->_parent.getID() == INVALID_INDEX)
		{
		transform->_model = Matrix::identity;
		}
		else
		{
		transform->_model = (*m_components)[transform->_parent.getID()]._model;
		}

		transform->_model = transform->_model.translate(transform->position);
		transform->_model = transform->_model.rotate(transform->rotation);
		transform->_model = transform->_model.scale(transform->scale);
		}

		//Update next child
		for(auto it = transform->_children->iterator(); it; ++it)
		{
		updateRecursive(it.value());
		}
		//*/
	}

	void ILARGIA_COMPONENT_MANAGER_NAME(Transform)::onComponentAdded(Entity* entity, Component& component)
	{
		/*
		Transform* t = component;

		uint32_t chunk = 8;
		Global::retrieve<uint32_t>("MEMCHUNK_TRANSFORM_CHILDREN", &chunk);
		m_rootTransforms->add(component);
		//*/
	}

	void ILARGIA_COMPONENT_MANAGER_NAME(Transform)::onComponentRemoved(Entity* entity, Component& component)
	{
		/*
		for(auto it = m_rootTransforms->iterator(); it; ++it)
		{
		Component c = it.value();
		if(c == component)
		{
		Transform* t = component;
		t->flags |= ObjectType::REQUIRE_DELETE;
		return;
		}
		}
		//*/
	}

	Component ILARGIA_COMPONENT_MANAGER_NAME(Transform)::createComponent()
	{
		return setupComponent<Transform>(m_components->add());
	}

	void ILARGIA_COMPONENT_MANAGER_NAME(Transform)::destroyComponent(Component& component)
	{
		//m::i32 i = component.getInstanceIndex();
		//_components->remove(i);
		/*
		MUON_ASSERT(component.isValid()
		, "[MODULE] (%s) Trying to destroy an Invalid Component!"
		, _name);
		if(Transform* ptr = component)
		{
		//Make its children have a new parent
		for(auto it = ptr->_children->iterator(); it; ++it)
		{
		(*m_components)[it.index()]._parent = makeComponent();
		}
		m_components->remove(component);
		ptr->Transform::~Transform();
		//Invalidate the component
		component = makeComponent();
		ptr->_parent = component;
		}
		//*/
	}

	void* ILARGIA_COMPONENT_MANAGER_NAME(Transform)::getComponent(m::i32 index)
	{
		return &m_components->get(index);
	}

	Component ILARGIA_COMPONENT_MANAGER_NAME(Transform)::getComponent(void* object)
	{
		for (m::i32 i = 0; i < m_components->size(); ++i)
		{
			if (object == &(m_components->get(i)))
			{
				return setupComponent<Transform>(i);
			}
		}
		return Component();
	}
}
