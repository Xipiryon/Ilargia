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
	IBaseManager::IBaseManager(const m::String& name, m::u64 componentType, m::i32 updateOrder)
		: _log(name)
		, _managerName(name)
		, _componentType(componentType)
		, _updateOrder(updateOrder)
	{
	}

	IBaseManager::~IBaseManager()
	{
	}

	const m::String& IBaseManager::getManagerName() const
	{
		return _managerName;
	}

	m::u64 IBaseManager::getComponentType() const
	{
		return _componentType;
	}

	m::i32 IBaseManager::getUpdateOrder() const
	{
		return _updateOrder;
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
