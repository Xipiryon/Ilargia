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

#ifndef INCLUDE_ILARGIA_VECTOR_HPP
#define INCLUDE_ILARGIA_VECTOR_HPP

#include <Muon/System/Log.hpp>
#include "Ilargia/Core/Define.hpp"

namespace ilg
{
	//! 3D Vector class
	class ILARGIA_API Vector
	{
	public:

		//! X coordinate
		m::f32 x;
		//! Y coordinate
		m::f32 y;
		//! Z coordinate
		m::f32 z;

		//! Default constructor
		Vector(m::f32 x = 0.f, m::f32 y = 0.f, m::f32 z = 0.f);

		/*!
		* @brief Return vector length
		* If used for comparison, prefer the use of
		* ::squareLength(), this is faster as there is no
		* square root computation
		*/
		m::f32 length() const;

		//! Return vector square length
		m::f32 squareLength() const;

		//! Return a new vector normalized
		Vector normalize() const;

		//! Return the dot product of two vectors
		static m::f32 dot(const Vector& u, const Vector& v);

		//! Return the cross product of two vectors
		static Vector cross(const Vector& u, const Vector& v);

		//! Return a new vector resulting of the lerp between u and v
		static Vector lerp(const Vector& u, const Vector& v, m::f32 t);

		//! Scalar multiplication
		Vector operator*(const m::f32 s) const;
		//! Scalar division
		Vector operator/(const m::f32 s) const;

		//! Vector addition
		Vector operator+(const Vector& v) const;
		//! Vector substraction
		Vector operator-(const Vector& v) const;

		//! Self-scalar multiplcation
		Vector& operator*=(const m::f32 s);
		//! Self-scalar division
		Vector& operator/=(const m::f32 s);

		//! Self-vector addition
		Vector& operator+=(const Vector& v);
		//! Self-vector substraction
		Vector& operator-=(const Vector& v);

		//! Return true if vectors are equal
		bool operator==(const Vector& v) const;
		//! Return true if vectors are different
		bool operator!=(const Vector& v) const;
	};
}

m::system::Log& operator<<(m::system::Log& stream, const ilg::Vector& v);
//m::memory::Stream& operator<<(m::memory::Stream& stream, const ilg::Vector& v);
//m::memory::Stream& operator>>(m::memory::Stream& stream, const ilg::Vector& v);

#endif
