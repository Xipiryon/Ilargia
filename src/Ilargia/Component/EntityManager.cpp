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

#include <Muon/System/Assert.hpp>
#include "Ilargia/Component/EntityManager.hpp"

namespace ilg
{
	EntityManager::EntityManager()
	{
		typedef std::deque<Entity*> EntityDeque;
		_entities = MUON_NEW(EntityDeque);
	}

	EntityManager::~EntityManager()
	{
		MUON_DELETE(_entities);
	}

	Entity* EntityManager::create()
	{
		Entity* e = MUON_NEW(Entity);
		_entities->push_back(e);
		return e;
	}

	void EntityManager::destroy(Entity* e)
	{
		for (auto it = _entities->begin(); it != _entities->end(); ++it)
		{
			if ((*it) == e)
			{
				_entities->erase(it);
				break;
			}
		}
		MUON_DELETE(e);
	}
}
