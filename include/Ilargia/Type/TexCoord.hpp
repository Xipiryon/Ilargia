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

#ifndef INCLUDE_ILARGIA_TEXCOORD_HPP
#define INCLUDE_ILARGIA_TEXCOORD_HPP

#include <Muon/System/Log.hpp>
#include "Ilargia/Core/Define.hpp"

namespace ilg
{
	class ILARGIA_API TexCoord
	{
	public:

		m::f32 u;
		m::f32 v;

		TexCoord(m::f32 u = 0.f, m::f32 v = 0.f);

		bool operator==(const TexCoord& t) const;
		bool operator!=(const TexCoord& t) const;
		TexCoord operator*(const m::f32 s) const;
		TexCoord operator/(const m::f32 s) const;

		TexCoord operator*(const TexCoord& t) const;
		TexCoord operator/(const TexCoord& t) const;
		TexCoord operator+(const TexCoord& t) const;
		TexCoord operator-(const TexCoord& t) const;

		TexCoord operator*=(const m::f32 s);
		TexCoord operator/=(const m::f32 s);

		TexCoord operator*=(const TexCoord& t);
		TexCoord operator/=(const TexCoord& t);
		TexCoord operator+=(const TexCoord& t);
		TexCoord operator-=(const TexCoord& t);

		static TexCoord lerp(const TexCoord& u, const TexCoord& v, m::f32 t);
	};
}

m::system::Log& operator<<(m::system::Log& stream, const ilg::TexCoord& t);

#endif
