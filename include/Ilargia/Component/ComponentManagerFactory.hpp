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

#ifndef _ILARGIA_COMPONENTMANAGERFACTORY_H_INCLUDED
#define _ILARGIA_COMPONENTMANAGERFACTORY_H_INCLUDED

#include <Muon/Helper/Singleton.hpp>
#include "Ilargia/Component/Entity.hpp"
#include "Ilargia/Component/Component.hpp"
#include "Ilargia/Component/ComponentManagerFactory.hpp"

namespace ilg
{
	class ILARGIA_API ComponentManagerFactory : public m::helper::Singleton<ComponentManagerFactory>
	{
		friend class m::helper::Singleton<ComponentManagerFactory>;
	public:

		template<typename T, typename ...Args>
		bool registerComponentManager(Args...args)
		{
			if (checkComponentManager(T::name()))
			{
				//TODO Fix it
				//return registerComponentManager(MUON_NEW(T, std::forward<Args>(args)...), T::name());
			}
			return false;
		}

		IComponentManager* getComponentManager(m::u64 componentType);
		IComponentManager* getComponentManager(const m::String& name);

	private:
		bool checkComponentManager(const m::String&);
		bool registerComponentManager(IComponentManager*, const m::String&);

		ComponentManagerFactory();
		ComponentManagerFactory(const ComponentManagerFactory&);
		ComponentManagerFactory& operator = (const ComponentManagerFactory&);
		virtual ~ComponentManagerFactory();
	};

#define ILARGIA_CMANAGER_TYPE(Manager, ComponentType) ((Manager*)ComponentManagerFactory::getInstance().getComponentManager(ComponentType))
#define ILARGIA_CMANAGER_NAME(Manager, Name) ((Manager*)ComponentManagerFactory::getInstance().getComponentManager(Name))

}
#endif
