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
#include "Ilargia/Type/Color.hpp"

namespace ilg
{
	Color::Color(m::f32 r_, m::f32 g_, m::f32 b_, m::f32 a_)
		: r(r_)
		, g(g_)
		, b(b_)
		, a(a_)
	{
	}

	bool Color::operator==(const Color& v) const
	{
		return (r == v.r
				&& g == v.g
				&& b == v.b
				&& a == v.a);
	}

	bool Color::operator!=(const Color& v) const
	{
		return !operator==(v);
	}

	// Self Scalar operation
	Color Color::operator*=(const m::f32 s)
	{
		r *= s;
		g *= s;
		b *= s;
		a *= s;
		return *this;
	}

	Color Color::operator/=(const m::f32 s)
	{
		*this *= (1.f / s);
		return *this;
	}

	// Scalar operation
	Color Color::operator*(const m::f32 s) const
	{
		return Color(r*s, g*s, b*s, a*s);
	}

	Color Color::operator/(const m::f32 s) const
	{
		return (*this * (1.f / s));
	}

	//Self Color operation
	Color Color::operator*=(const Color& v)
	{
		r *= v.r;
		g *= v.g;
		b *= v.b;
		a *= v.a;
		return *this;
	}

	Color Color::operator/=(const Color& v)
	{
		r /= v.r;
		g /= v.g;
		b /= v.b;
		a /= v.a;
		return *this;
	}

	Color Color::operator+=(const Color& v)
	{
		r += v.r;
		g += v.g;
		b += v.b;
		a += v.a;
		return *this;
	}

	Color Color::operator-=(const Color& v)
	{
		r -= v.r;
		g -= v.g;
		b -= v.b;
		a -= v.a;
		return *this;
	}

	//Color operation
	Color Color::operator*(const Color& v) const
	{
		return Color(r*v.r, g*v.g, b*v.b, a*v.a);
	}

	Color Color::operator/(const Color& v) const
	{
		return Color(r / v.r, g / v.g, b / v.b, a / v.a);
	}

	Color Color::operator+(const Color& v) const
	{
		return Color(r + v.r, g + v.g, b + v.b, a + v.a);
	}

	Color Color::operator-(const Color& v) const
	{
		return Color(r - v.r, g - v.g, b - v.b, a + v.a);
	}

	Color Color::lerp(const Color& u, const Color& v, m::f32 t)
	{
		return (v - u)*t + u;
	}
}

/*
	memory::IStream& Color::operator<<(memory::IStream& stream)
	{
	return stream << r << g << b << a;
	}

	memory::IStream& Color::operator>>(memory::IStream& stream)
	{
	return stream >> r >> g >> b >> a;
	}
	//*/

m::system::Log& operator<<(m::system::Log& stream, const ilg::Color& c)
{
	return stream << "[" << c.r << ", " << c.g << ", " << c.b << ", " << c.a << "]";
}
