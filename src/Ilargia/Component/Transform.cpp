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

#include <Muon/System/Assert.hpp>

#include "Ilargia/Manager/ManagerFactory.hpp"
#include "Ilargia/Component/TransformManager.hpp"
#include "Ilargia/Component/Transform.hpp"

namespace ilg
{
	Transform::Transform()
		: position(0.f, 0.f, 0.f)
		, scale(1.f, 1.f, 1.f)
		, rotation(Quaternion::identity)
		, m_model(Matrix::identity)
		, m_children(MUON_NEW(ChildArray))
	{
	}

	Transform::~Transform()
	{
		MUON_DELETE(m_children);
	}

	void Transform::setParent(Component parent)
	{
		TransformManager* manager = ILARGIA_COMPONENT_MANAGER_TYPE(TransformManager, MUON_META(Transform)->id());

		//Remove myself from my previous parent if any
		if (m_parent.instanceId() != m::INVALID_INDEX)
		{
			Transform* myParent = m_parent;
			myParent->m_children->remove(manager->getComponent(this).instanceId());
		}

		if (m_parent.instanceId() != m::INVALID_INDEX)
		{
			//Notify my parent it has a new child!
			Transform* tParent = parent;
			tParent->m_children->add(manager->getComponent(this));
		}

		//Either valid or not
		m_parent = parent;
		manager->requireRootListUpdate();
	}

	void Transform::addChild(Component child)
	{
		MUON_ASSERT(child.instanceId() != m::INVALID_INDEX, "Adding an invalid child!");
		if (child.instanceId() != m::INVALID_INDEX)
		{
			Transform* childPtr = child;
			Transform* childParentPtr = (childPtr->m_parent.instanceId() == m::INVALID_INDEX ? NULL : (Transform*)childPtr->m_parent);
			//Don't update parent if we're already the one or if NULL
			if (childParentPtr != NULL && childParentPtr != this)
			{
				childParentPtr->m_children->remove(child.instanceId());
			}
			m_children->add(child);
			ILARGIA_COMPONENT_MANAGER_TYPE(TransformManager, MUON_META(Transform)->id())->requireRootListUpdate();
		}
	}
}
