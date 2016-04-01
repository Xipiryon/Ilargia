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

// ***  SYSTEM  ***
#include <Muon/System/Time.hpp>

// ***  CORE    ***
#include "Ilargia/Engine.hpp"
#include "SharedLibrary.hpp"

// ***  TYPE    ***
#include <Muon/String.hpp>
#include "Ilargia/Type/Vector.hpp"
#include "Ilargia/Type/TexCoord.hpp"
#include "Ilargia/Type/Color.hpp"
#include "Ilargia/Type/Matrix.hpp"
#include "Ilargia/Type/Quaternion.hpp"

// **** COMPONENTS ***
#include "Ilargia/Component/Transform.hpp"

namespace ilg
{
	void Engine::_registerCoreClass()
	{
		m::meta::MetaDatabase& db = m::meta::MetaDatabase::getInstance();
		db.registerMeta<ilg::Component>();
		db.registerMeta<ilg::Transform>();
	}

	void Engine::_registerCoreComponentManager()
	{
		ilg::manager::ManagerFactory& factory = ilg::manager::ManagerFactory::getInstance();
		factory.registerComponentManager(MUON_NEW(ILARGIA_COMPONENT_MANAGER_NAME(Transform)));
	}
}
