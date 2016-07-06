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

#ifndef INCLUDE_ILARGIA_OPENGLCOMPONENT_CAMERA_HPP
#define INCLUDE_ILARGIA_OPENGLCOMPONENT_CAMERA_HPP

#include <Ilargia/Type/Vector.hpp>
#include <Ilargia/Type/Matrix.hpp>

namespace ilg
{
	namespace graphics
	{
		class ILARGIA_API Camera
		{
		public:
			~Camera();

			void setPerspective(
				float fov,
				float aspect,
				float near,
				float far);

			void setOrthographic(
				float left,
				float right,
				float bottom,
				float top,
				float near,
				float far);

			void lookAt(
				const Vector& position,
				const Vector& center,
				const Vector& up);

			Matrix setFrustum(
				float left,
				float right,
				float bottom,
				float top,
				float near,
				float far);

			Vector getPosition() const;
			Vector getCenter() const;
			Vector getUp() const;

			Matrix getViewMatrix() const;
			Matrix getProjectionMatrix() const;

			bool isOrthographic() const;

		protected:
			friend class CameraModule;
			Camera();

			Vector m_position;
			Vector m_direction;
			Vector m_up;

			Matrix m_viewMatrix;
			Matrix m_projMatrix;
			bool m_isOrtho;
			float m_near;
			float m_far;
		};
	}
}
#endif
