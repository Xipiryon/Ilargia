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

#include "Ilargia/Component/IComponentManager.hpp"
#include "Ilargia/Component/ComponentManagerFactory.hpp"
#include "Ilargia/Component/Component.hpp"

namespace ilg
{
	Component::Component()
		: _type(MUON_META(Component)->id())
		, _instance(muon::INVALID_INDEX)
		, _name(MUON_META(Component)->name())
	{
	}

	Component::Component(muon::u64 type, muon::i32 instance, const muon::String& name)
		: _type(type)
		, _instance(instance)
		, _name(name)
	{
	}

	Component::Component(const Component& c)
		: _type(c._type)
		, _instance(c._instance)
		, _name(c._name)
	{
	}

	Component& Component::operator=(const Component& c)
	{
		if (this != &c)
		{
			_type = c._type;
			_instance = c._instance;
			_name = c._name;
		}
		return *this;
	}


	muon::i32 Component::instanceId() const
	{
		return _instance;
	}
	
	muon::u64 Component::instanceType() const
	{
		return _type;
	}

	muon::String Component::instanceName() const
	{
		return _name;
	}

	void* Component::_cast(muon::u64 type, const char* type_name) const
	{
		MUON_ASSERT(_instance != muon::INVALID_INDEX, "Component instance is invalid!");
		MUON_ASSERT(type != muon::INVALID_INDEX, "Trying to cast to an invalid type!");
		if (type == muon::INVALID_INDEX
			|| _instance == muon::INVALID_INDEX)
		{
			return NULL;
		}

		IComponentManager* manager = ComponentManagerFactory::get().getComponentManager(type);
		MUON_ASSERT(manager != NULL,
			"There is no ComponentManager matching the type %u (%s)",
			type, type_name);

		if(manager != NULL)
		{
			MUON_ASSERT(type == _type,
				"Cast Type (%s) does not match Component Type (%s)",
				type_name, _type);
			if(type == _type)
			{
				return manager->getComponent(_instance);
			}
		}
		return NULL;
	}
}
