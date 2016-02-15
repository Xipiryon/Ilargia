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

#include <algorithm>
#include <fstream>

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

// ***  SYSTEM  ***
#include <Muon/System/Time.hpp>

namespace ilg
{
	bool Engine::_registerClasses()
	{
		// ********
		// Registering reflection
		// This time, order doesn't really matter.
		// 
		// Registering a type will allow to instantiate it from its name or id.
		// and thus, must inherit from "reflect::Object"
		//
		// *** Type
#if 0
		auto& odb = reflect::ObjectDatabase::getInstance();
		odb.registerClass<Vector>();
		odb.registerClass<Color>();
		odb.registerClass<TexCoord>();
		odb.registerClass<Matrix>();
		odb.registerClass<Quaternion>();

		// *** System
		odb.registerClass<String>();
#endif
		return true;
	}
}