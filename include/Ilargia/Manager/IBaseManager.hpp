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

#ifndef INCLUDE_ILARGIA_IBASEMANAGER_HPP
#define INCLUDE_ILARGIA_IBASEMANAGER_HPP

#include <Muon/Helper/NonCopyable.hpp>
#include <Muon/System/Log.hpp>
#include "Ilargia/Component/Entity.hpp"
#include "Ilargia/Component/Component.hpp"

namespace ilg
{
	namespace manager
	{
		class ManagerFactory;
		class ILARGIA_API IBaseManager : public m::helper::NonCopyable
		{
		protected:
			IBaseManager();
			IBaseManager(const m::String& name, m::u64 componentType, m::i32 updateOrder);
		public:
			virtual ~IBaseManager();

			const m::String&	getManagerName() const;
			m::u64			getComponentType() const;
			m::i32			getUpdateOrder() const;

			virtual void onInit() = 0;
			virtual void onUpdate(m::f32 deltaTime) = 0;
			virtual void onTerm() = 0;

			virtual void onKeyCallback(void* windowHandle, int key, int scancode, int action, int modifier) = 0;

			virtual void onComponentAdded(Entity* entity, Component& component) = 0;
			virtual void onComponentRemoved(Entity* entity, Component& component) = 0;

			virtual Component createComponent() = 0;
			virtual void destroyComponent(Component& component) = 0;
			virtual void* getComponent(m::i32 index) = 0;
			virtual Component getComponent(void* object) = 0;

			virtual void onEntityHierarchyChanged(Entity* entity, Entity* previousParent, Entity* newParent) = 0;
		protected:
			template<typename T>
			Component setupComponent(m::i32 instance)
			{
				return Component(MUON_META(T)->id(), instance, MUON_META(T)->name());
			}

			m::system::Log& getLog()
			{
				return m_log();
			}

			m::system::Log& getLog(m::LogLevel level)
			{
				return m_log(level);
			}

		private:
			friend class ManagerFactory;
			m::system::Log m_log;
			m::String	m_managerName;
			m::u64		m_componentType;
			m::i32		m_updateOrder;
		};
	}
}
#endif
