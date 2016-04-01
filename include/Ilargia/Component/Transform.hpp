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

#ifndef INCLUDE_ILARGIA_TRANSFORM_HPP
#define INCLUDE_ILARGIA_TRANSFORM_HPP

#include <Muon/Helper/NonCopyable.hpp>
#include <Muon/Traits/TypeTraits.hpp>
#include "Ilargia/Component/ComponentStorage.hpp"
#include "Ilargia/Component/Component.hpp"

#include "Ilargia/Type/Vector.hpp"
#include "Ilargia/Type/Quaternion.hpp"
#include "Ilargia/Type/Matrix.hpp"

namespace ilg
{
	class MeshRendererModule;
	class ILARGIA_API Transform
	{
	public:
		Transform();
		~Transform();

		Vector position;
		Vector scale;
		Quaternion rotation;

		void setParent(Component parent);

	private:
		friend class TransformModule;
		friend class MeshRendererModule;

		void addChild(Component child);

		Matrix		m_model;
		Component	m_parent;
		typedef ComponentStorage<Component, 8> ChildArray;
		ChildArray* m_children;
	};
}
MUON_TRAITS_DECL(ilg::Transform);

#endif
