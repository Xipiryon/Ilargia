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
#include <Muon/Memory/Allocator.hpp>
#include "Ilargia/Component/Entity.hpp"
#include "Ilargia/Component/IComponentManager.hpp"
#include "Ilargia/Component/ComponentManagerFactory.hpp"

namespace ilg
{
	Entity::Entity()
	{
		typedef Array<Component, 8> ComponentArray;
		_components = MUON_NEW(ComponentArray);
	}

	Entity::~Entity()
	{
		MUON_DELETE(_components);
	}

	Component Entity::_addComponent(muon::i32 type)
	{
		IComponentManager* manager = ComponentManagerFactory::get().getComponentManager(type);
		Component c;
		if (manager)
		{
			c = manager->createComponent();
			_components->add(c);
			manager->onComponentAdd(this, c);
		}
		return c;
	}

	Component Entity::_getComponent(muon::i32 type)
	{
		for (muon::i32 i = 0; i < _components->size(); ++i)
		{
			Component c = _components->get(i);
			if (c.instanceType() == type)
			{
				return c;
			}
		}
		return Component();
	}

	bool Entity::_removeComponent(muon::i32 type)
	{
		IComponentManager* manager = ComponentManagerFactory::get().getComponentManager(type);
		Component c;
		if (manager)
		{
			for (muon::i32 i = 0; i < _components->size(); ++i)
			{
				Component c = _components->get(i);
				if (c.instanceType() == type)
				{
					manager->onComponentRemove(this, c);
					manager->destroyComponent(c);
					_components->remove(i);
					return true;
				}
			}
		}
		return false;
	}
}
