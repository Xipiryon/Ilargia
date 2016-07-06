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

#ifndef INCLUDE_ILARGIA_OPENGLCOMPONENT_FREECAMERA_HPP
#define INCLUDE_ILARGIA_OPENGLCOMPONENT_FREECAMERA_HPP

#if 0
#include <SFML/Window/Event.hpp>
#include "Camera.h"
#include "Frustum.h"

namespace sf
{
	class RenderWindow;
}

namespace okz
{
	class FreeCamera : public Singleton<FreeCamera>
	{
		friend class Singleton<FreeCamera>;

	private:
		FreeCamera();
		~FreeCamera();

	public:
		float getSpeed();
		void getSensivity(float&, float&);
		okz::fVertex getPosition() const;
		void getAngle(float&, float&);

		void setSpeed(float);
		void setSensivity(float, float);
		void setMinMaxAngle(float, float);
		void setAngle(float, float);
		void setPosition(okz::fVertex);
		void reset();

		okz::fVertex compute(const sf::Event&);
		void animate(float);

		Frustum frustum;
	protected:
		okz::Camera* _camera;
		sf::RenderWindow* _window;

		std::pair<float, float> _sensivity;
		float _speed;

		float _hAngle;
		float _vAngle;
		float _min;
		float _max;

		float _deltaTime;
		bool _animate;

		okz::fVertex _lastPosition;

		okz::fVertex _position;
		okz::fVertex _target;
		okz::fVertex _forward;
		okz::fVertex _side;

		//Mouse specific
		bool _forceMouse;
		bool _shift;
		okz::fVertex _center;
		okz::fVertex _mouse;
	};
}
#endif

#endif
