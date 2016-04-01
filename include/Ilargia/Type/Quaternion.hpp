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

#ifndef INCLUDE_ILARGIA_QUATERNION_HPP
#define INCLUDE_ILARGIA_QUATERNION_HPP

#include <Muon/System/Log.hpp>
#include "Ilargia/Type/Matrix.hpp"

namespace ilg
{
	/*!
	* @brief Quaternion represent a rotation
	* Quaternion don't suffer from Gimbal Lock. They're used in the engine
	* to represent rotation.
	* The x,y,z,w attributes are complex numbers, and do not
	* directly represent rotation around axis.
	*/
	class ILARGIA_API Quaternion
	{
	public:

		//! Equivalent to Quaternion(0, 0, 0, 1) (x,y,z,w)
		static const Quaternion Identity;

		m::f32 x; //! X Attribute
		m::f32 y; //! Y Attribute
		m::f32 z; //! Z Attribute
		m::f32 w; //! W attribute

		//! Default constructor
		Quaternion(m::f32 x = 0.f, m::f32 y = 0.f, m::f32 z = 0.f, m::f32 w = 1.f);

		//! Return true if quaternion are equal
		bool operator==(const Quaternion& q) const;
		//! Return true if quaternion are different
		bool operator!=(const Quaternion& q) const;

		/*!
		* @brief Multiply with another Quaternion
		* @param q Another quaternion
		* @return A new Quaternion, result of the multiplication
		*/
		Quaternion operator*(const Quaternion& q) const;

		/*!
		* @brief Apply the rotation stored by the quaternion to a Vector
		* @param v A vector to be transformed
		* @return The Vector transformed by the current quaternion
		*/
		Vector operator*(const Vector& v) const;

		//! Return the conjugate of a Quaternion
		Quaternion conjugate() const;

		//! Return a normalized version of a Quaternion
		Quaternion normalize() const;

		//! Return the inverse of a Quaternion
		Quaternion inverse() const;

		//! Return the square length of a Quaternion (faster than length())
		m::f32 squareLength() const;

		//! Return the length of a Quaternion
		m::f32 length() const;

		/*!
		* @brief Create a Quaternion from a rotation matrix
		*
		* Create a quaternion from a rotation matrix.
		*
		* @param[in] matrix Rotation matrix.
		* @return A Quaternion.
		*/
		static Quaternion fromMatrix(const Matrix& matrix);

		/*!
		* @brief Create a Quaternion from Euler angle
		*
		* Create a quaternion representing a Euler rotation.
		*
		* @param[in] rotation In that order: z degrees around z axis, y degrees around y axis, x degrees around x axis
		* @return A Quaternion.
		*/
		static Quaternion fromEuler(const Vector& rotation);

		/*!
		* @brief Create a Quaternion from Angle Axis
		*
		* Create a quaternion representing a rotation of 'angle' degrees
		* around a Vector(x,y,z) axis.
		*
		* @param[in] angle Amount of rotation in degrees
		* @param[in] axis Axis around which the rotation is made
		* @return A Quaternion.
		*/
		static Quaternion fromAngleAxis(m::f32 angle, const Vector& axis);

		/*!
		* @brief Euler angle of this quaternion
		*
		* Create a vector representing an euler rotation for a give quaternion.
		* The rotation order is xyz.
		*
		* @return A vector representing the Euler angle of the rotation
		*/
		Vector toEuler()  const;

		/*!
		* @brief Angle Axis of this quaternion
		*
		* Create a vector representing an euler rotation for a give quaternion.
		*
		* @param[in, out] angle The angle of this quaternion
		* @return A vector where x,y,z are the rotation on each axis.
		*/
		Vector toAngleAxis(m::f32& angle) const;

		//! Create a Rotation matrix equivalent to the quaternion
		Matrix toMatrix() const;
	};
}

m::system::Log& operator<<(m::system::Log& stream, const ilg::Quaternion& q);

#endif
