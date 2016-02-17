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

#ifndef _ILARGIA_COLOR_H_INCLUDED
#define _ILARGIA_COLOR_H_INCLUDED

#include <Muon/System/Log.hpp>
#include "Ilargia/Core/Define.hpp"

namespace ilg
{
	class ILARGIA_API Color
	{
	public:

		muon::f32 r;
		muon::f32 g;
		muon::f32 b;
		muon::f32 a;

		Color(muon::f32 r = 0.f, muon::f32 g = 0.f, muon::f32 b = 0.f, muon::f32 a = 0.f);

		bool operator==(const Color& v) const;
		bool operator!=(const Color& v) const;
		Color operator*(const muon::f32 s) const;
		Color operator/(const muon::f32 s) const;

		Color operator*(const Color& v) const;
		Color operator/(const Color& v) const;
		Color operator+(const Color& v) const;
		Color operator-(const Color& v) const;

		Color operator*=(const muon::f32 s);
		Color operator/=(const muon::f32 s);

		Color operator*=(const Color& v);
		Color operator/=(const Color& v);
		Color operator+=(const Color& v);
		Color operator-=(const Color& v);

		static Color lerp(const Color& u, const Color& v, muon::f32 t);
	};
}

muon::system::Log& operator<<(muon::system::Log& stream, const ilg::Color& c);

#endif
