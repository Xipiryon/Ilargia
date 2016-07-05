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
#include "OpenGLComponent/Camera.hpp"

namespace ilg
{
	namespace graphics
	{
		Camera::Camera()
			: m_position(1.0f, 1.0f, 1.0f)
			, m_direction(0.0f, 0.0f, 0.0f)
			, m_up(0.0f, 1.0f, 0.0f)
		{
		}

		Camera::~Camera()
		{
		}

		Vector Camera::getPosition() const
		{
			return m_position;
		}

		Vector Camera::getCenter() const
		{
			return m_direction;
		}

		Vector Camera::getUp() const
		{
			return m_up;
		}

		Matrix Camera::getViewMatrix() const
		{
			return m_viewMatrix;
		}

		Matrix Camera::getProjectionMatrix() const
		{
			return m_projMatrix;
		}

		bool Camera::isOrthographic() const
		{
			return m_isOrtho;
		}

		void Camera::setPerspective(float fov
									, float ratio
									, float near
									, float far)
		{
			const float rad = std::tan(fov * m::PI_f / 360.f);

			m_projMatrix = Matrix::Zero;
			m_projMatrix.x.x = 1.f / (ratio * rad);
			m_projMatrix.y.y = 1.f / rad;
			m_projMatrix.z.z = -(far + near) / (far - near);
			m_projMatrix.z.w = -1;
			m_projMatrix.w.z = -(2.f * far * near) / (far - near);
			m_isOrtho = false;
			m_near = near;
			m_far = far;
		}

		void Camera::setOrthographic(float left
									 , float right
									 , float bottom
									 , float top
									 , float near
									 , float far)
		{
			m_projMatrix = Matrix::Identity;
			m_projMatrix.x.x = 2.f / (right - left);
			m_projMatrix.y.y = 2.f / (top - bottom);
			m_projMatrix.z.z = -2.f / (far - near);
			m_projMatrix.w.x = -(right + left) / (right - left);
			m_projMatrix.w.y = -(top + bottom) / (top - bottom);
			m_projMatrix.w.z = -(far + near) / (far - near);
			m_isOrtho = true;
			m_near = near;
			m_far = far;
		}

		void Camera::lookAt(const Vector& pos
							, const Vector& center
							, const Vector& up)
		{
			m_position = pos;
			m_direction = center;
			m_up = up;
			/*
			*		OpenGL MODELVIEW
			*		Column Major
			*	X.x		Y.x		Z.x		T.x
			*	X.x		Y.y		Z.y		T.y
			*	X.x		Y.z		Z.z		T.z
			*	-		-		-		-
			* X: Side
			* Y: Up
			* Z: Forward
			* T: Translation
			*/

			const Vector forward = (pos - center).normalize();
			// Side = forward x up
			const Vector side = (Vector::cross(up, forward)).normalize();

			// Recompute up as: up = side x forward
			const Vector rup = Vector::cross(forward, side);

			//Work on temp matrix
			Matrix view = Matrix::Identity;

			view.x.x = side.x;
			view.y.x = side.y;
			view.z.x = side.z;

			view.x.y = rup.x;
			view.y.y = rup.y;
			view.z.y = rup.z;

			view.x.z = forward.x;
			view.y.z = forward.y;
			view.z.z = forward.z;

			view.w.x = -Vector::dot(side, pos);
			view.w.y = -Vector::dot(rup, pos);
			view.w.z = -Vector::dot(forward, pos);

			m_viewMatrix = view;
		}

		Matrix Camera::setFrustum(float left
								  , float right
								  , float bottom
								  , float top
								  , float near
								  , float far)
		{
			Matrix result = Matrix::Zero;
			result.x.x = (2.f * near) / (right - left);
			result.y.y = (2.f * near) / (top - bottom);
			result.z.x = (right + left) / (right - left);
			result.z.y = (top + bottom) / (top - bottom);
			result.z.z = -(far + near) / (far - near);
			result.z.w = -1.f;
			result.w.z = -(2.f * far * near) / (far - near);

			return result;
		}
	}
}
