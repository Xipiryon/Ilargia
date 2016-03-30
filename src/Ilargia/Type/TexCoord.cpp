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

#include <cstdio>
#include "Ilargia/Type/TexCoord.hpp"

namespace ilg
{
	TexCoord::TexCoord(m::f32 u_, m::f32 v_)
		: u(u_)
		, v(v_)
	{
	}

	bool TexCoord::operator==(const TexCoord& t) const
	{
		return (u == t.u
				&& t == t.v);
	}

	bool TexCoord::operator!=(const TexCoord& t) const
	{
		return !operator==(v);
	}

	// Self Scalar operation
	TexCoord TexCoord::operator*=(const m::f32 s)
	{
		u *= s;
		v *= s;
		return *this;
	}

	TexCoord TexCoord::operator/=(const m::f32 s)
	{
		*this *= (1.f / s);
		return *this;
	}

	// Scalar operation
	TexCoord TexCoord::operator*(const m::f32 s) const
	{
		return TexCoord(u*s, v*s);
	}

	TexCoord TexCoord::operator/(const m::f32 s) const
	{
		return (*this * (1.f / s));
	}

	//Self TexCoord operation
	TexCoord TexCoord::operator*=(const TexCoord& t)
	{
		u *= t.u;
		v *= t.v;
		return *this;
	}

	TexCoord TexCoord::operator/=(const TexCoord& t)
	{
		u /= t.u;
		v /= t.v;
		return *this;
	}

	TexCoord TexCoord::operator+=(const TexCoord& t)
	{
		u += t.u;
		v += t.v;
		return *this;
	}

	TexCoord TexCoord::operator-=(const TexCoord& t)
	{
		u -= t.u;
		v -= t.v;
		return *this;
	}

	//TexCoord operation
	TexCoord TexCoord::operator*(const TexCoord& t) const
	{
		return TexCoord(u*t.u, v*t.v);
	}

	TexCoord TexCoord::operator/(const TexCoord& t) const
	{
		return TexCoord(u / t.u, v / t.v);
	}

	TexCoord TexCoord::operator+(const TexCoord& t) const
	{
		return TexCoord(u + t.u, v + t.v);
	}

	TexCoord TexCoord::operator-(const TexCoord& t) const
	{
		return TexCoord(u - t.u, v - t.v);
	}

	TexCoord TexCoord::lerp(const TexCoord& u, const TexCoord& v, m::f32 t)
	{
		return (v - u)*t + u;
	}
}
/*
memory::IStream& TexCoord::operator<<(memory::IStream& stream)
{
return stream << u << v;
}

memory::IStream& TexCoord::operator>>(memory::IStream& stream)
{
return stream >> u >> v;
}
//*/

m::system::Log& operator<<(m::system::Log& stream, const ilg::TexCoord& t)
{
	return stream << "[" << t.u << ", " << t.v << "]";
}
