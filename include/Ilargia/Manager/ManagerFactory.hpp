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

#ifndef INCLUDE_ILARGIA_MANAGERFACTORY_HPP
#define INCLUDE_ILARGIA_MANAGERFACTORY_HPP

#include <Muon/Helper/Singleton.hpp>
#include "Ilargia/Manager/IBaseManager.hpp"

namespace ilg
{
	class ILARGIA_API ManagerFactory : public m::helper::Singleton<ManagerFactory>
	{
		friend class m::helper::Singleton<ManagerFactory>;
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

		IBaseManager* getComponentManager(m::u64 componentType);
		IBaseManager* getComponentManager(const m::String& name);

	private:
		bool checkComponentManager(const m::String&);
		bool registerComponentManager(IBaseManager*, const m::String&);

		ManagerFactory();
		ManagerFactory(const ManagerFactory&);
		ManagerFactory& operator = (const ManagerFactory&);
		virtual ~ManagerFactory();
	};

#define ILARGIA_CMANAGER_TYPE(Manager, ComponentType) ((Manager*)ManagerFactory::getInstance().getComponentManager(ComponentType))
#define ILARGIA_CMANAGER_NAME(Manager, Name) ((Manager*)ManagerFactory::getInstance().getComponentManager(Name))
}
#endif
