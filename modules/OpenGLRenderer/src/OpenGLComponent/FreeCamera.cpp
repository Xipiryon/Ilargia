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

#if 0
#include "Graphics.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include "FreeCamera.h"
#include "Input.h"

//#ifdef WIN32
#define M_PI 3.14159265358979323846264338327
//#endif

okz::FreeCamera::FreeCamera() :
_camera(okz::Camera::get()),
_window(okz::Graphics::get()->getWindowPtr()),
_speed(100),
_sensivity(std::pair<float, float>(50, 100)),
_deltaTime(0),
_animate(false),
_shift(false)
{
	_center.x = _window->getSize().x / 2.f;
	_center.y = _window->getSize().y / 2.f;
}

okz::FreeCamera::~FreeCamera()
{
}

float okz::FreeCamera::getSpeed()
{
	return _speed;
}

void okz::FreeCamera::getSensivity(float& h, float& v)
{
	h = _sensivity.first;
	v = _sensivity.second;
}

okz::fVertex okz::FreeCamera::getPosition() const
{
	return _position;
}

void okz::FreeCamera::getAngle(float& h, float& v)
{
	h = _hAngle;
	v = _vAngle;
}

void okz::FreeCamera::setSpeed(float s)
{
	_speed = s;
}

void okz::FreeCamera::setSensivity(float h, float v)
{
	_sensivity = std::pair<float, float>(h, v);
}

void okz::FreeCamera::setPosition(okz::fVertex p)
{
	_position = p;
	_animate = true;
}

void okz::FreeCamera::setMinMaxAngle(float min, float max)
{
	_min = min;
	_max = max;
}

void okz::FreeCamera::setAngle(float horizontal, float vertical)
{
	_hAngle = horizontal;
	_vAngle = (vertical > _max ? _max : vertical < _min ? _min : vertical);
	_animate = true;
}

void okz::FreeCamera::reset()
{
	_position = _lastPosition;
}

okz::fVertex okz::FreeCamera::compute(const sf::Event& event)
{
	//Shift is released
	if (event.type == sf::Event::KeyReleased)
	{
		if (okz::Input::get()->getInput("Shift", event))
		{
			_shift = false;
			_window->setMouseCursorVisible(true);
		}
	}

	// Mouse moved with SHIFT pressed
	if (event.type == sf::Event::MouseMoved && _shift)
	{
		if (_forceMouse)
		{
			_animate = true;
			_forceMouse = false;
			float x = event.mouseMove.x;
			float y = event.mouseMove.y;
			x -= _mouse.x;
			y -= _mouse.y;

			float delta = 0.75f;
			if (x > delta || x < -delta)
				_hAngle -= -x * (_sensivity.first * _deltaTime);
			if (y > delta || y < -delta)
				_vAngle -= y * (_sensivity.second * _deltaTime);

			_mouse.x = _center.x;
			_mouse.y = _center.y;

			sf::Mouse::setPosition(sf::Vector2i(_center.x, _center.y), *_window);
		}
		else
			_forceMouse = true;
	}

	// Wheel -- Vertical movement
	if (event.type == sf::Event::MouseWheelMoved)
	{
		_animate = true;
		_position.y += event.mouseWheel.delta * (_speed * _deltaTime);
	}

	//Key pressed
	if (event.type == sf::Event::KeyPressed)
	{
		okz::Input* input = okz::Input::get();
		//"Normal" movement
		if (input->getInput("Up", event))
		{
			_position += _forward * (_speed * _deltaTime);
			_animate = true;
		}
		if (input->getInput("Down", event))
		{
			_position -= _forward * (_speed * _deltaTime);
			_animate = true;
		}
		if (input->getInput("Left", event))
		{
			_position += _side * (_speed * _deltaTime);
			_animate = true;
		}
		if (input->getInput("Right", event))
		{
			_position -= _side * (_speed * _deltaTime);
			_animate = true;
		}
		if (input->getInput("-", event))
		{
			_position.y -= (_speed * _deltaTime);
			_animate = true;
		}
		if (input->getInput("+", event))
		{
			_position.y += (_speed * _deltaTime);
			_animate = true;
		}

		//Shift is pressed
		if (input->getInput("Shift", event))
		{
			if (!_shift)
			{
				_shift = true;
				_forceMouse = false;
				_mouse.x = _center.x;
				_mouse.y = _center.y;
				_window->setMouseCursorVisible(false);
				sf::Mouse::setPosition(sf::Vector2i(_center.x, _center.y), *_window);
			}
		}
	}
	return _position;
}

void okz::FreeCamera::animate(float deltaTime)
{
	static const okz::fVertex up(0.f, 1.f, 0.f);

	if (_animate)
	{
		_deltaTime = deltaTime;
		_lastPosition = _position;
		_camera->setEye(_position);

		if (_vAngle >= _max)
			_vAngle = _max;
		else if (_vAngle <= _min)
			_vAngle = _min;

		//Spherical coord into cartesian coord
		double temp = cos(_vAngle*M_PI / 180);
		_forward.y = sin(_vAngle*M_PI / 180);
		_forward.x = temp*cos(_hAngle*M_PI / 180);
		_forward.z = temp*sin(_hAngle*M_PI / 180);

		_side.crossProduct(up, _forward);
		_side.normalize();

		_target = _position + _forward;
		_camera->setLook(_target);
		//Reset forward, so that we only move on the plane of the eye
		_forward.y = 0.f;
		_forward.x = cos(_hAngle*M_PI / 180);
		_forward.z = sin(_hAngle*M_PI / 180);
		_animate = false;

		//Compute Frustum
		frustum.updateFrustum();
	}
}
#endif
