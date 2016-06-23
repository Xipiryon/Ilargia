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

#include <cmath>
#include <cstdio>
#include "Ilargia/Type/Vector.hpp"

namespace ilg
{
	Vector::Vector(m::f32 x_, m::f32 y_, m::f32 z_)
		: x(x_)
		, y(y_)
		, z(z_)
	{
	}

	m::f32 Vector::length() const
	{
		return ::sqrt(squareLength());
	}

	m::f32 Vector::squareLength() const
	{
		return x*x + y*y + z*z;
	}

	Vector Vector::normalize() const
	{
		Vector v(x, y, z);
		m::f32 len = v.length();
		if (len != 0)
		{
			m::f32 coef = 1.f / len;
			v.x *= coef;
			v.y *= coef;
			v.z *= coef;
		}
		return v;
	}

	m::f32 Vector::dot(const Vector& u, const Vector& v)
	{
		return u.x*v.x + u.y*v.y + u.z*v.z;
	}

	Vector Vector::cross(const Vector& u, const Vector& v)
	{
		Vector uv;
		uv.x = (u.y*v.z) - (u.z*v.y);
		uv.y = (u.z*v.x) - (u.x*v.z);
		uv.z = (u.x*v.y) - (u.y*v.x);
		return uv;
	}

	Vector Vector::lerp(const Vector& u, const Vector& v, m::f32 t)
	{
		return (v - u)*t + u;
	}

	bool Vector::operator==(const Vector& v) const
	{
		return (x == v.x
				&& y == v.y
				&& z == v.z);
	}

	bool Vector::operator!=(const Vector& v) const
	{
		return !operator==(v);
	}

	Vector Vector::operator*(const m::f32 s) const
	{
		return Vector(x*s, y*s, z*s);
	}

	Vector Vector::operator/(const m::f32 s) const
	{
		return (*this * (1.f / s));
	}

	Vector Vector::operator+(const Vector& v) const
	{
		return Vector(x + v.x, y + v.y, z + v.z);
	}

	Vector Vector::operator-(const Vector& v) const
	{
		return Vector(x - v.x, y - v.y, z - v.z);
	}

	Vector& Vector::operator*=(const m::f32 s)
	{
		x *= s;
		y *= s;
		z *= s;
		return *this;
	}

	Vector& Vector::operator/=(const m::f32 s)
	{
		operator*=(1.f / s);
		return *this;
	}

	Vector& Vector::operator+=(const Vector& v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}

	Vector& Vector::operator-=(const Vector& v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}
}

m::system::Log& operator<<(m::system::Log& stream, const ilg::Vector& v)
{
	return stream << "[" << v.x << ", " << v.y << ", " << v.z << "]";
}

/*
m::memory::Stream& operator<<(m::memory::Stream& stream, const ilg::Vector& v)
{
return stream << v.x << v.y << v.z;
}
m::memory::Stream& operator>>(m::memory::Stream& stream, const ilg::Vector& v);
{
return stream >> v.x >> v.y >> v.z;
}
//*/
