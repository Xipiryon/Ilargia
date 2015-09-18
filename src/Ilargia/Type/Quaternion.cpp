/*************************************************************************
* DNA Engine - http://gitweb.louis-schnellbach.com
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

#include <Muon/System/Log.hpp>

#include "Ilargia/System/Script.hpp"
#include "Ilargia/Type/Quaternion.hpp"

namespace ilg
{

	const Quaternion Quaternion::identity = {0, 0, 0, 1};

	Quaternion::Quaternion(muon::f32 _x, muon::f32 _y, muon::f32 _z, muon::f32 _w)
		: x(_x)
		, y(_y)
		, z(_z)
		, w(_w)
	{
	}

	bool Quaternion::operator==(const Quaternion& p) const
	{
		return (w == p.w && Vector(x,y,z) == Vector(p.x, p.y, p.z));
	}

	bool Quaternion::operator!=(const Quaternion& p) const
	{
		return !operator==(p);
	}

	Quaternion Quaternion::operator*(const Quaternion& p) const
	{
		Quaternion qp;
		Vector vI = Vector(x,y,z);
		Vector vP = Vector(p.x,p.y,p.z);
		qp.w = w * p.w - Vector::dot(vP,vI);
		Vector v = vI*w + vP*w + Vector::cross(vI,vP);
		qp.x = v.x;
		qp.y = v.y;
		qp.z = v.z;
		return qp;
	}

	Vector Quaternion::operator*(const Vector& v) const
	{
		return toMatrix() * v;
	}


	Quaternion Quaternion::conjugate() const
	{
		Quaternion qc;
		qc.x = -x;
		qc.y = -y;
		qc.z = -z;
		qc.w = w;
		return qc;
	}

	Quaternion Quaternion::normalize() const
	{
		muon::f32 l = length();
		Quaternion qn;
		qn.w = w / l;
		qn.x = x / l;
		qn.y = y / l;
		qn.z = z / l;
		return qn;
	}

	Quaternion Quaternion::inverse() const
	{
		muon::f32 ls = squareLength();
		Quaternion qi = conjugate();
		qi.w = qi.w / ls;
		qi.x = qi.x / ls;
		qi.y = qi.y / ls;
		qi.z = qi.z / ls;
		return qi;
	}


	muon::f32 Quaternion::squareLength() const
	{
		return w * w + Vector(x,y,z).squareLength();
	}

	muon::f32 Quaternion::length() const
	{
		return ::sqrt(squareLength());
	}

	Quaternion Quaternion::fromEuler(const Vector& rotation)
	{
		Quaternion q;
		Vector rad = rotation / 180.f * muon::PI_f;
		//Half cos and half sin
		Vector c(
			::cos(0.5f * rad.x),
			::cos(0.5f * rad.y),
			::cos(0.5f * rad.z)
		);
		Vector s(
			::sin(0.5f * rad.x),
			::sin(0.5f * rad.y),
			::sin(0.5f * rad.z)
		);

		q.x = s.x*c.y*c.z - c.x*s.y*s.z;
		q.y = c.x*s.y*c.z + s.x*c.y*s.z;
		q.z = c.x*c.y*s.z - s.x*s.y*c.z;
		q.w = c.x*c.y*c.z + s.x*s.y*s.z;
		return q;
	}

	Quaternion Quaternion::fromAngleAxis(muon::f32 angle, const Vector& axis)
	{
		// cos( theta / 2) , sin( theta / 2)
		//			w			x,y,z
		//angle are in degrees, let's convert in rad
		// 360 deg = 2pi rad
		muon::f32 rad = angle / 180.f * muon::PI_f;
		Quaternion q;
		Vector nAxis = axis*::sin(rad * 0.5f);
		q.x = nAxis.x;
		q.y = nAxis.y;
		q.z = nAxis.z;
		q.w = ::cos(rad * 0.5f);
		return q;
	}

	Quaternion Quaternion::fromMatrix(const Matrix& matrix)
	{
		Quaternion q = {0.f, 0.f, 0.f, 1.f};
		muon::f32 trace = matrix.x.x + matrix.y.y + matrix.z.z + 1.f;
		if(trace > 0.f)
		{
			muon::f32 s = 0.5f / ::sqrt(trace);
			q.w = 0.25f / s;
			//			m[9]		m[6]
			q.x = (matrix.z.y - matrix.y.z) * s;
			//			m[2]		m[8]
			q.y = (matrix.x.z - matrix.z.x) * s;
			//			m[4]		m[1]
			q.z = (matrix.y.x - matrix.x.y) *s;
		}
		else
		{
			muon::f32 major[4] =
			{
				matrix.x.x,
				matrix.y.y,
				matrix.z.z,
				matrix.w.w,
			};

			muon::f32 s = 0.f;
			if(major[0] > major[1] && major[0] > major[2])
			{
				s = ::sqrtf(1.f + matrix.x.x - matrix.y.y - matrix.z.z) * 2.f;
				q.x = 0.5f / s;
				q.y = (matrix.y.x + matrix.x.y) * s;
				q.z = (matrix.x.z + matrix.z.x) * s;
				q.w = (matrix.z.y + matrix.y.z) * s;
			}
			else if(major[1] > major[2])
			{
				s = ::sqrtf(1.f - matrix.x.x + matrix.y.y - matrix.z.z) * 2.f;
				q.x = (matrix.y.x + matrix.x.y) * s;
				q.y = 0.5f / s;
				q.z = (matrix.z.y + matrix.y.z) * s;
				q.w = (matrix.x.z + matrix.z.x) * s;
			}
			else
			{
				s = ::sqrtf(1.f - matrix.x.x - matrix.y.y + matrix.z.z) * 2.f;
				q.x = (matrix.x.z + matrix.z.x) * s;
				q.y = (matrix.z.y + matrix.y.z) * s;
				q.z = 0.5f / s;
				q.w = (matrix.y.x + matrix.x.y) * s;
			}
		}
		return q;
	}

	Vector Quaternion::toAngleAxis(muon::f32& angle) const
	{
		const Quaternion& q = *this;
		angle = ::acos(q.w);
		Vector v = Vector(x,y,z) * (1.f / ::sin(angle));
		angle *= 360.f / muon::PI_f;
		return v;
	}

	MUON_INLINE muon::f32 _pitch(const Quaternion& q)
	{
		return ::atan2(2.f * (q.y*q.z + q.x*q.w),
					   q.w*q.w - q.x*q.x - q.y*q.y + q.z*q.z);
	}

	MUON_INLINE muon::f32 _yaw(const Quaternion& q)
	{
		return ::asin(-2.f * (q.x*q.z - q.y*q.w));
	}

	MUON_INLINE muon::f32 _roll(const Quaternion& q)
	{
		return ::atan2(2.f * (q.x*q.y + q.z*q.w),
					   q.w*q.w + q.x*q.x - q.y*q.y - q.z*q.z);
	}

	Vector Quaternion::toEuler() const
	{
		Vector v(_pitch(*this), _yaw(*this), _roll(*this));
		return (v / muon::PI_f * 180.f);
	}

	Matrix Quaternion::toMatrix() const
	{
		const Quaternion& q = *this;

		muon::f32 xx = q.x*q.x;
		muon::f32 xy = q.x*q.y;
		muon::f32 xz = q.x*q.z;
		muon::f32 xw = q.x*q.w;

		muon::f32 yy = q.y*q.y;
		muon::f32 yz = q.y*q.z;
		muon::f32 yw = q.y*q.w;

		muon::f32 zz = q.z*q.z;
		muon::f32 zw = q.z*q.w;

		MatrixRow rx =
		{
			1.f - 2.f * ( yy + zz ),
			2.f * ( xy - zw ),
			2.f * ( xz + yw ),
			0.f
		};

		MatrixRow ry =
		{
			2.f * ( xy + zw ),
			1.f - 2.f * ( xx + zz ),
			2.f * ( yz - xw ),
			0.f
		};

		MatrixRow rz =
		{
			2.f * ( xz - yw ),
			2.f * ( yz + xw ),
			1.f - 2.f * ( xx + yy ),
			0.f
		};

		MatrixRow rw =
		{
			0.f,
			0.f,
			0.f,
			1.f
		};

		Matrix m(rx, ry, rz, rw);
		return m;
	}

	void Quaternion::registerScriptBinding(asIScriptEngine* engine)
	{
	}
}

/*
	memory::IStream& Quaternion::operator<<(memory::IStream& stream)
	{
	return stream << x << y << z << w;
	}

	memory::IStream& Quaternion::operator>>(memory::IStream& stream)
	{
	return stream >> x >> y >> z >> w;
	}
	//*/

muon::system::Log& operator<<(muon::system::Log& stream, const ilg::Quaternion& q)
{
	return stream << "[" << q.x << ", " << q.y << ", " << q.z << ", " << q.w << "]";
}
