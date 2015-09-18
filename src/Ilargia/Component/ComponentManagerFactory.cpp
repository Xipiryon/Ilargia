/*************************************************************************
* DNA Engine - http://gitweb.louis-schnellbach.com
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
#include "Ilargia/Component/IComponentManager.hpp"
#include "Ilargia/Component/ComponentManagerFactory.hpp"
#include "../SharedLibrary.hpp"

namespace ilg
{
	ComponentManagerFactory::ComponentManagerFactory()
	{
	}
	
	ComponentManagerFactory::~ComponentManagerFactory()
	{
	}

	bool ComponentManagerFactory::checkComponentManager(const muon::String& name)
	{
		//Check if manager isn't already loaded
		bool found = false;
		auto& managerList = SharedLibrary::get()._managers;
		for (auto it = managerList.begin(); it != managerList.end(); ++it)
		{
			if ((*it).manager->_managerName == name)
			{
				found = true;
				break;
			}
		}
		MUON_ASSERT(found == false, "Module \"%s\" alread added: aborting...\n", name.cStr());
		return !found;
	}

	bool ComponentManagerFactory::registerComponentManager(IComponentManager* manager, const muon::String& name)
	{
		//Module coulnd't be allocated
		MUON_ASSERT(manager != NULL, "Couldn't allocate manager \"%s\"!\n", name.cStr());
		if (manager == NULL)
		{
			return false;
		}

		SharedLibrary::get()._addModuleRef(manager);
		return true;
	}

	IComponentManager* ComponentManagerFactory::getComponentManager(muon::u64 type)
	{
		auto& managerList = SharedLibrary::get()._managers;
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

	IComponentManager* ComponentManagerFactory::getComponentManager(const muon::String& name)
	{
		auto& managerList = SharedLibrary::get()._managers;
		for (auto it = managerList.begin(); it != managerList.end(); ++it)
		{
			if ((*it).manager->_managerName == name)
			{
				return (*it).manager;
				break;
			}
		}
		return NULL;
	}
}
