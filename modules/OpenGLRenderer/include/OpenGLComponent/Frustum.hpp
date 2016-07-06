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

#ifndef INCLUDE_ILARGIA_OPENGLCOMPONENT_FRUSTUM_HPP
#define INCLUDE_ILARGIA_OPENGLCOMPONENT_FRUSTUM_HPP

#if 0
#include "Vertex.h"

namespace okz
{
	class Frustum
	{
	public:
		void updateFrustum()
		{
			GLfloat projection[16];
			GLfloat modelview[16];
			GLfloat mvp[16];

			/* Get the current PROJECTION and MODELVIEW matrices from OpenGL */
			glGetFloatv(GL_PROJECTION_MATRIX, projection);
			glGetFloatv(GL_MODELVIEW_MATRIX, modelview);

			glPushMatrix();
			//Load the stored projection matrix
			glLoadMatrixf(projection);
			//multiply the stored MODELVIEW matrix with the projection matrix
			glMultMatrixf(modelview);
			//we read the result of the multiplication
			glGetFloatv(GL_MODELVIEW_MATRIX, mvp);
			//and restore the former MODELVIEW_MATRIX
			glPopMatrix();

			/* Extract the RIGHT plane */
			_planes[PLANE_RIGHT] = extractPlane(mvp[3] - mvp[0],
												mvp[7] - mvp[4],
												mvp[11] - mvp[8],
												mvp[15] - mvp[12]);

			/* Extract the LEFT plane */
			_planes[PLANE_LEFT] = extractPlane(mvp[3] + mvp[0],
											   mvp[7] + mvp[4],
											   mvp[11] + mvp[8],
											   mvp[15] + mvp[12]);

			/* Extract the TOP plane */
			_planes[PLANE_TOP] = extractPlane(mvp[3] - mvp[1],
											  mvp[7] - mvp[5],
											  mvp[11] - mvp[9],
											  mvp[15] - mvp[13]);

			/* Extract the BOTTOM plane */
			_planes[PLANE_BOTTOM] = extractPlane(mvp[3] + mvp[1],
												 mvp[7] + mvp[5],
												 mvp[11] + mvp[9],
												 mvp[15] + mvp[13]);

			/* Extract the FAR plane */
			_planes[PLANE_FAR] = extractPlane(mvp[3] - mvp[2],
											  mvp[7] - mvp[6],
											  mvp[11] - mvp[10],
											  mvp[15] - mvp[14]);

			/* Extract the NEAR plane */
			_planes[PLANE_NEAR] = extractPlane(mvp[3] + mvp[2],
											   mvp[7] + mvp[6],
											   mvp[11] + mvp[10],
											   mvp[15] + mvp[14]);
		}

		bool sphereInFrustum(okz::fVertex v, float radius)
		{
			for (int p = 0; p < 6; p++)
			{
				if (_planes[p].a * v.x +
					_planes[p].b * v.y +
					_planes[p].c * v.z +
					_planes[p].d <= -radius)
				{
					return false;
				}
			}
			return true;
		}

		bool pointInFrustum(okz::fVertex v)
		{
			for (int p = 0; p < 6; p++)
			{
				if (_planes[p].a * v.x +
					_planes[p].b * v.y +
					_planes[p].c * v.z +
					_planes[p].d < 0)
				{
					return false;
				}
			}
			return true;
		}

		bool cubeInFrustum(okz::fVertex v, float size)
		{
			bool result;
			// for each plane do ...
			for (int i = 0; i < 6; i++)
			{
				// reset counters for corners in and out
				int out = 0;
				int in = 0;
				// for each corner of the box do ...
				// get out of the cycle as soon as a box as corners
				// both inside and out of the frustum
				for (int k = 0; k < 8 && (in == 0 || out == 0); k++)
				{
					// is the corner outside or inside
					if (_planes[i].distance(v) < 0)
						out++;
					else
						in++;
				}
				//if all corners are out
				if (!in)
					return false;
				// if some corners are out and others are in
				else if (out)
					result = true;
			}
			return result;
		}

	private:
		struct Plane
		{
			float a, b, c, d;
			float distance(const okz::fVertex& v)
			{
				return a * v.x + b * v.y + c * v.z + d;
			}
		};
		Plane _planes[6];

		enum
		{
			PLANE_LEFT = 0,
			PLANE_RIGHT,
			PLANE_TOP,
			PLANE_BOTTOM,
			PLANE_FAR,
			PLANE_NEAR
		};

		Plane extractPlane(float a, float b, float c, float d)
		{
			float t = sqrt(a * a + b * b + c * c);
			Plane p;
			p.a = a / t;
			p.b = b / t;
			p.c = c / t;
			p.d = d / t;
			return p;
		}
	};
}
#endif

#endif
