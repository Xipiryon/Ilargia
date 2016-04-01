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

#include "Ilargia/Manager/IComponentManager.hpp"
#include "Ilargia/Manager/ManagerFactory.hpp"
#include "Ilargia/Component/Component.hpp"

namespace ilg
{
	Component::Component()
		: m_instanceTypeId(MUON_META(Component)->id())
		, m_instanceIndex(m::INVALID_INDEX)
		, m_instanceName(MUON_META(Component)->name())
	{
	}

	Component::Component(m::u64 type, m::i32 instance, const m::String& name)
		: m_instanceTypeId(type)
		, m_instanceIndex(instance)
		, m_instanceName(name)
	{
	}

	Component::Component(const Component& c)
		: m_instanceTypeId(c.m_instanceTypeId)
		, m_instanceIndex(c.m_instanceIndex)
		, m_instanceName(c.m_instanceName)
	{
	}

	Component& Component::operator=(const Component& c)
	{
		if (this != &c)
		{
			m_instanceTypeId = c.m_instanceTypeId;
			m_instanceIndex = c.m_instanceIndex;
			m_instanceName = c.m_instanceName;
		}
		return *this;
	}

	m::i32 Component::getInstanceIndex() const
	{
		return m_instanceIndex;
	}

	m::u64 Component::getInstanceTypeId() const
	{
		return m_instanceTypeId;
	}

	m::String Component::getInstanceName() const
	{
		return m_instanceName;
	}

	void* Component::_cast(m::u64 type, const char* type_name) const
	{
		MUON_ASSERT(m_instanceIndex != m::INVALID_INDEX, "Component instance is invalid!");
		MUON_ASSERT(type != m::INVALID_INDEX, "Trying to cast to an invalid type!");
		if (type == m::INVALID_INDEX
			|| m_instanceIndex == m::INVALID_INDEX)
		{
			return NULL;
		}

		manager::IBaseManager* manager = manager::ManagerFactory::getInstance().getComponentManager(type);
		MUON_ASSERT(manager != NULL,
					"There is no ComponentManager matching the type %u (%s)",
					type, type_name);

		if (manager != NULL)
		{
			MUON_ASSERT(type == m_instanceTypeId,
						"Cast Type (%s) does not match Component Type (%s)",
						type_name, m_instanceTypeId);
			if (type == m_instanceTypeId)
			{
				return manager->getComponent(m_instanceIndex);
			}
		}
		return NULL;
	}
}
