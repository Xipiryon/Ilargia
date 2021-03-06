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

#include <Muon/System/Log.hpp>
#include <Muon/System/Assert.hpp>
#include "Ilargia/Manager/IBaseManager.hpp"
#include "Ilargia/Manager/ManagerFactory.hpp"
#include "../SharedLibrary.hpp"

namespace ilg
{
	namespace manager
	{
		ManagerFactory::ManagerFactory()
		{
		}

		ManagerFactory::~ManagerFactory()
		{
		}

		bool ManagerFactory::registerComponentManager(IBaseManager* manager)
		{
			MUON_ASSERT_BREAK(manager != NULL, "Can't register a non allocated manager!");
			if (checkComponentManager(manager->getManagerName()))
			{
				SharedLibrary::getInstance()._addModuleRef(manager);
				return true;
			}
			return false;
		}

		bool ManagerFactory::checkComponentManager(const m::String& name)
		{
			//Check if manager isn't already loaded
			bool found = false;
			auto& managerList = SharedLibrary::getInstance().m_managers;
			for (auto it = managerList.begin(); it != managerList.end(); ++it)
			{
				if ((*it).manager->m_managerName == name)
				{
					found = true;
					break;
				}
			}
			MUON_ASSERT(found == false, "Module \"%s\" alread added: aborting...\n", name.cStr());
			return !found;
		}

		IBaseManager* ManagerFactory::getComponentManager(m::u64 type)
		{
			auto& managerList = SharedLibrary::getInstance().m_managers;
			for (auto it = managerList.begin(); it != managerList.end(); ++it)
			{
				if ((*it).manager->getComponentType() == type)
				{
					return (*it).manager;
					break;
				}
			}
			return NULL;
		}

		IBaseManager* ManagerFactory::getComponentManager(const m::String& name)
		{
			auto& managerList = SharedLibrary::getInstance().m_managers;
			for (auto it = managerList.begin(); it != managerList.end(); ++it)
			{
				if ((*it).manager->m_managerName == name)
				{
					return (*it).manager;
					break;
				}
			}
			return NULL;
		}

		m::u32 ManagerFactory::getComponentManagerCount() const
		{
			return SharedLibrary::getInstance().m_managers.size();
		}

		IBaseManager* ManagerFactory::getComponentManagerFromIndex(m::u32 index)
		{
			auto& managerList = SharedLibrary::getInstance().m_managers;
			MUON_ASSERT_BREAK(index < managerList.size(), "No Manager at index %d!", index);
			return managerList[index].manager;
		}
	}
}
