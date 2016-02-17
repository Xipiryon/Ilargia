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

#ifndef _ILARGIA_MATRIX_H_INCLUDED
#define _ILARGIA_MATRIX_H_INCLUDED

#include "Ilargia/Type/Vector.hpp"

namespace ilg
{
	class Quaternion;

	//! Float array containing a row from a 4x4 Matrix
	class ILARGIA_API MatrixRow
	{
	public:
		//! X coordinate of the MatrixRow
		muon::f32 x;
		//! Y coordinate of the MatrixRow
		muon::f32 y;
		//! Z coordinate of the MatrixRow
		muon::f32 z;
		//! W coordinate of the MatrixRow
		muon::f32 w;

		MatrixRow(muon::f32 x = 0.f, muon::f32 y = 0.f, muon::f32 z = 0.f, muon::f32 w = 0.f);

		//! Acces the {n}th attribute by copy
		muon::f32 operator[](muon::i32 i) const;

		//! Acces the {n}th attribute by reference
		muon::f32& operator[](muon::i32 i);

		//! Return true if MatrixRow are equal
		bool operator==(const MatrixRow& m) const;
		//! Return true if MatrixRow are different
		bool operator!=(const MatrixRow& m) const;
	};

	/*!
	* @brief A 4x4 muon::f32 Matrix
	*
	* The matrix 1D array representation is:
	* <table>
	*	<tr><td></td><td>.x</td><td>.y</td><td>.z</td><td>.w</td></tr>
	*	<tr><td>x</td><td>0</td><td>1</td><td>2</td><td>3</td></tr>
	*	<tr><td>y</td><td>4</td><td>5</td><td>6</td><td>7</td></tr>
	*	<tr><td>z</td><td>8</td><td>9</td><td>10</td><td>11</td></tr>
	*	<tr><td>w</td><td>12</td><td>13</td><td>14</td><td>15</td></tr>
	* </table>
	*
	* Where each line is a MatrixRow, and each column correspond
	* to the x/y/z/w attribute of the row:
	*
	* Matrix.x.z = 2 , Matrix.z.x = 8 ...
	*/
	class ILARGIA_API Matrix
	{
	public:

		//! 4x4 Identity Matrix
		static const Matrix identity;
		//! 4x4 Zero Matrix
		static const Matrix zero;

		//! First row (Index array are [0..3])
		MatrixRow x;
		//! Second row  (Index array are [4..7])
		MatrixRow y;
		//! Third row  (Index array are [8..11])
		MatrixRow z;
		//! Fourth row  (Index array are [12..15])
		MatrixRow w;

		Matrix(MatrixRow x = MatrixRow()
				, MatrixRow y = MatrixRow()
				, MatrixRow z = MatrixRow()
				, MatrixRow w = MatrixRow());

		//! Acces the {n}th row by copy
		MatrixRow operator[](muon::i32 i) const;

		//! Acces the {n}th row by reference
		MatrixRow& operator[](muon::i32 i);

		//! Return the matrix determinant
		muon::f32 determinant() const;

		//! Return the transpose matrix
		Matrix transpose() const;

		//! Return the inverse matrix
		Matrix inverse() const;

		/*!
		* @brief Apply a translation of Vector v
		* @param v A vector representing the translation
		*/
		void translate(const Vector& v);

		/*!
		* @brief Apply a rotation of Quaternion q
		* @param q A quaternion representing the rotation
		*/
		void rotate(const Quaternion& q);

		/*!
		* @brief Apply a scale of Vector s
		* @param s A vector representing the scale factor
		*/
		void scale(const Vector& s);

		/*!
		* @brief Apply the transformation stored by the matrix to a Vector
		* @param v A vector to be transformed
		* @return The Vector transformed by the current matrix
		*/
		Vector operator*(const Vector& v) const;

		/*!
		* @brief Multiply with another Matrix
		* @param m Another matrix
		* @return A new Matrix, result of the multiplication
		*/
		Matrix operator*(const Matrix& m) const;

		/*!
		* @brief Multiply every element of the matrix with a scalar
		* @param k A scalar
		* @return A new Matrix, result of the multiplication
		*/
		Matrix operator*(muon::f32 k) const;

		/*!
		* @brief Divide every element of the matrix with a scalar
		* @param k A scalar
		* @return A new Matrix, result of the division
		* @note If k happens to be 0, then no division occurs
		*/
		Matrix operator/(muon::f32 k) const;

		//! Return true if matrices are equal
		bool operator==(const Matrix& m) const;
		//! Return true if matrices are different
		bool operator!=(const Matrix& m) const;
	};
}

//muon::memory::Stream& operator<<(muon::memory::Stream& stream);
//muon::memory::Stream& operator>>(muon::memory::Stream& stream);
muon::system::Log& operator<<(muon::system::Log& stream, const ilg::MatrixRow& r);

//muon::memory::Stream& operator<<(muon::memory::Stream& stream);
//muon::memory::Stream& operator>>(muon::memory::Stream& stream);
muon::system::Log& operator<<(muon::system::Log& stream, const ilg::Matrix& m);

#endif
