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

#ifndef _ILARGIA_ICOMPONENTMANAGER_H_INCLUDED
#define _ILARGIA_ICOMPONENTMANAGER_H_INCLUDED

#include <Muon/Helper/NonCopyable.hpp>
#include <Muon/System/Log.hpp>
#include "Ilargia/Component/Entity.hpp"
#include "Ilargia/Component/Component.hpp"

namespace ilg
{
	class ComponentManagerFactory;
	class ILG_API IComponentManager : public muon::helper::NonCopyable
	{
	public:
		IComponentManager(const muon::String& name, muon::u64 componentType, muon::i32 updateOrder);
		virtual ~IComponentManager();

		const muon::String&	getManagerName() const;
		muon::u64			getComponentType() const;
		muon::i32			getUpdateOrder() const;

		virtual bool onInit() = 0;
		virtual bool onUpdate(muon::f32 deltaTime) = 0;
		virtual bool onTerm() = 0;

		virtual void onKeyCallback(void* windowHandle, int key, int scancode, int action, int modifier);
		virtual void onComponentAdd(Entity* entity, Component& component);
		virtual void onComponentRemove(Entity* entity, Component& component);

		virtual Component createComponent() = 0;
		virtual void destroyComponent(Component& component) = 0;
		virtual void* getComponent(muon::i32 index) = 0;
		virtual Component getComponent(void* object) = 0;

	protected:
		IComponentManager();
		template<typename T>
		Component setupComponent(muon::i32 instance)
		{
			return Component(MUON_META(T)->id(), instance, MUON_META(T)->name());
		}

		muon::system::Log _log;

	private:
		friend class ComponentManagerFactory;
		muon::String	_managerName;
		muon::u64		_componentType;
		muon::i32		_updateOrder;
	};
}
#endif
