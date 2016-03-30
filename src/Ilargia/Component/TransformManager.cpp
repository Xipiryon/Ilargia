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

#include <Muon/System/Log.hpp>

#include "Ilargia/Component/TransformManager.hpp"
#include "Ilargia/Component/Transform.hpp"

namespace ilg
{
	TransformManager::TransformManager()
		: IComponentManager(MUON_META(Transform)->name(), MUON_META(Transform)->id(), 160)
		, _requireRootListUpdate(true)
		, _transforms(NULL)
		, _roots(NULL)
	{
	}

	TransformManager::~TransformManager()
	{
	}

	bool TransformManager::onInit()
	{
		/*
		uint32_t chunk = 64;
		Global::retrieve<uint32_t>("MEMCHUNK_TRANSFORM", &chunk);
		_transforms = new Array<Transform>(chunk);
		_roots = new Array<Component>(chunk);
		Log::debug("[TRANSFORM] Initialized (%d)", chunk);
		//*/
		return true;
	}

	bool TransformManager::onUpdate(m::f32 dt)
	{
		/*
		updateRootList();
		for (auto it = _roots->iterator(); it; ++it)
		{
		//Fast retrieve of the Transform* from the Component
		//(We don't go with it.value()->cast<>()...
		updateRecursive(it.value());
		}
		//*/
		return true;
	}

	bool TransformManager::onTerm()
	{
		/*
		if(_transforms != NULL)
		{
		for(auto it = _transforms->iterator(); it; ++it)
		{
		it.value().~Transform();
		}
		delete _transforms;
		_transforms = NULL;
		}

		if(_roots != NULL)
		{
		delete _roots;
		_roots = NULL;
		}
		//*/
		return true;
	}

	void TransformManager::requireRootListUpdate()
	{
		_requireRootListUpdate = true;
	}

	void TransformManager::updateRootList()
	{
		if (_requireRootListUpdate)
		{
			/*
			//Recreate the whole list
			m::i32 transform_type = type_id<Transform>();
			_roots->clear();
			for (auto it = _transforms->iterator(); it; ++it)
			{
			Transform& t = it.value();
			//Only had if the Transform have no valid parent
			if (!t._parent.isValid())
			{
			_roots->add(makeComponent(it.index(), transform_type));
			}
			}
			//*/
		}
	}

	void TransformManager::updateRecursive(Component& component)
	{
		/*
		Transform* transform = &(*_transforms)[component.getID()];
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
		transform->_model = (*_transforms)[transform->_parent.getID()]._model;
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

	void TransformManager::callbackNewComponent(Entity* entity, Component& component)
	{
		/*
		Transform* t = component;

		uint32_t chunk = 8;
		Global::retrieve<uint32_t>("MEMCHUNK_TRANSFORM_CHILDREN", &chunk);
		_roots->add(component);
		//*/
	}

	void TransformManager::callbackRemoveComponent(Entity* entity, Component& component)
	{
		/*
		for(auto it = _roots->iterator(); it; ++it)
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

	Component TransformManager::createComponent()
	{
		return setupComponent<Transform>(_transforms->add());
	}

	void TransformManager::destroyComponent(Component& component)
	{
		//m::i32 i = component.instanceId();
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
		(*_transforms)[it.index()]._parent = makeComponent();
		}
		_transforms->remove(component);
		ptr->Transform::~Transform();
		//Invalidate the component
		component = makeComponent();
		ptr->_parent = component;
		}
		//*/
	}

	void* TransformManager::getComponent(m::i32 index)
	{
		return &_transforms->get(index);
	}

	Component TransformManager::getComponent(void* object)
	{
		for (m::i32 i = 0; i < _transforms->size(); ++i)
		{
			if (object == &(_transforms->get(i)))
			{
				return setupComponent<Transform>(i);
			}
		}
		return Component();
	}
}
