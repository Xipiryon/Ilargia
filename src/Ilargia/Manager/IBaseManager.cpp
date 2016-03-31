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
#include <Muon/System/Assert.hpp>
#include "Ilargia/Manager/IBaseManager.hpp"

namespace ilg
{
	namespace manager
	{
		IBaseManager::IBaseManager(const m::String& name, m::u64 componentType, m::i32 updateOrder)
			: m_log(name)
			, m_managerName(name)
			, m_componentType(componentType)
			, m_updateOrder(updateOrder)
		{
		}

		IBaseManager::~IBaseManager()
		{
		}

		const m::String& IBaseManager::getManagerName() const
		{
			return m_managerName;
		}

		m::u64 IBaseManager::getComponentType() const
		{
			return m_componentType;
		}

		m::i32 IBaseManager::getUpdateOrder() const
		{
			return m_updateOrder;
		}

		void IBaseManager::onKeyCallback(void* windowHandle, int key, int scancode, int action, int modifier)
		{
		}

		void IBaseManager::onComponentAdd(Entity* entity, Component& component)
		{
		}

		void IBaseManager::onComponentRemove(Entity* entity, Component& component)
		{
		}
	}
}
