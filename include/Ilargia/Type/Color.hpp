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

#ifndef INCLUDE_ILARGIA_COLOR_HPP
#define INCLUDE_ILARGIA_COLOR_HPP

#include <Muon/System/Log.hpp>
#include "Ilargia/Core/Define.hpp"

namespace ilg
{
	class ILARGIA_API Color
	{
	public:

		m::f32 r;
		m::f32 g;
		m::f32 b;
		m::f32 a;

		Color(m::f32 r = 0.f, m::f32 g = 0.f, m::f32 b = 0.f, m::f32 a = 0.f);

		bool operator==(const Color& v) const;
		bool operator!=(const Color& v) const;
		Color operator*(const m::f32 s) const;
		Color operator/(const m::f32 s) const;

		Color operator*(const Color& v) const;
		Color operator/(const Color& v) const;
		Color operator+(const Color& v) const;
		Color operator-(const Color& v) const;

		Color operator*=(const m::f32 s);
		Color operator/=(const m::f32 s);

		Color operator*=(const Color& v);
		Color operator/=(const Color& v);
		Color operator+=(const Color& v);
		Color operator-=(const Color& v);

		static Color lerp(const Color& u, const Color& v, m::f32 t);
	};
}

m::system::Log& operator<<(m::system::Log& stream, const ilg::Color& c);

#endif
