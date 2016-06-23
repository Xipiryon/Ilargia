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

#ifndef INCLUDE_ILARGIA_ICOMPONENTMANAGER_HPP
#define INCLUDE_ILARGIA_ICOMPONENTMANAGER_HPP

#include "Ilargia/Manager/IBaseManager.hpp"

namespace ilg
{
	namespace manager
	{
		static const m::u32 ComponentManagerChunkSize = 64;
		template<typename ComponentType, m::u32 ChunkSize = ComponentManagerChunkSize>
		class IComponentManager : public IBaseManager
		{
			typedef ComponentStorage<ComponentType, ChunkSize> ComponentList;
		public:
			IComponentManager(m::i32 updateOrder)
				: IBaseManager(MUON_TRAITS_NAME(ComponentType), MUON_TRAITS_ID(ComponentType), updateOrder)
			{
				m_components = MUON_NEW(ComponentList);
			}

			virtual ~IComponentManager()
			{
			}

			virtual void onInit() = 0;
			virtual void onUpdate(m::f32 deltaTime) = 0;
			virtual void onTerm() = 0;

			virtual void onKeyCallback(void* windowHandle, int key, int scancode, int action, int modifier)
			{
			}

			virtual void onComponentAdded(Entity* entity, Component& component)
			{
			}

			virtual void onComponentRemoved(Entity* entity, Component& component)
			{
			}

			virtual Component createComponent() = 0;
			virtual void destroyComponent(Component& component) = 0;
			virtual void* getComponent(m::i32 index) = 0;
			virtual Component getComponent(void* object) = 0;

			virtual void onEntityHierarchyChanged(Entity* entity, Entity* previousParent, Entity* newParent) = 0;
		protected:
			ComponentList* m_components;
		};
	}
}
#define ILARGIA_COMPONENT_MANAGER_NAME(Component) Component##ComponentManager
#define ILARGIA_COMPONENT_DECL(Type) class ILARGIA_COMPONENT_MANAGER_NAME(Type); class ILARGIA_API Type : public ::ilg::Component
#define ILARGIA_COMPONENT_HAS_STATIC_MANAGER(Type, Name) static ILARGIA_COMPONENT_MANAGER_NAME(Type)* Name
#define ILARGIA_COMPONENT_FRIEND_MANAGER(Type) friend class ILARGIA_COMPONENT_MANAGER_NAME(Type)

#define ILARGIA_COMPONENT_MANAGER_DECL_SIZE(Component, ChunkSize) class ILARGIA_API ILARGIA_COMPONENT_MANAGER_NAME(Component) : public ::ilg::manager::IComponentManager<Component, ChunkSize>
#define ILARGIA_COMPONENT_MANAGER_DECL(Component) ILARGIA_COMPONENT_MANAGER_DECL_SIZE(Component, ilg::manager::ComponentManagerChunkSize)
#endif
