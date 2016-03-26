/*************************************************************************
* Ilargia Engine - http://github.com/Xleek/Ilargia
* C++ Modular Data Oriented Game Enginee
*------------------------------------------------------------------------
* Copyright (c) 2014 Louis Schnellbach
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

#include "ColorConsole.hpp"

#if defined(MUON_PLATFORM_WINDOWS)
#	include <windows.h>
#endif
#include <iostream>

namespace
{
	enum Color
	{
#if defined(MUON_PLATFORM_WINDOWS)
		C_DEFAULT = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
		C_DEBUG = FOREGROUND_BLUE | FOREGROUND_INTENSITY,
		C_WARNING = FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY,
		C_ERROR = FOREGROUND_RED | FOREGROUND_INTENSITY
#else
		C_DEFAULT = 37,
		C_DEBUG = 34,
		C_WARNING = 33,
		C_ERROR = 31
#endif
	};

	void color(std::ostream& o, m::LogLevel level)
	{
		Color c = C_DEFAULT;
		switch (level)
		{
			case m::LOG_DEBUG: c = C_DEBUG; break;
			case m::LOG_INFO: c = C_DEFAULT; break;
			case m::LOG_WARNING: c = C_WARNING; break;
			case m::LOG_ERROR: c = C_ERROR; break;
		}
#if defined(MUON_PLATFORM_WINDOWS)
		HANDLE consolehwnd = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(consolehwnd, c);
#else
		o << "\033[" << c << "m";
#endif
	}
}

namespace ilg
{
	namespace mod
	{
		ColorConsole::ColorConsole()
			: _level(m::LOG_INFO)
		{
		}

		ColorConsole::~ColorConsole()
		{
		}

		m::system::ILogImpl& ColorConsole::endl()
		{
			color(std::cout, m::LOG_INFO);
			std::cout << std::endl;
			return *this;
		}

		void ColorConsole::operator()(m::LogLevel level)
		{
			_level = level;
		}

		m::system::ILogImpl& ColorConsole::operator<<(const char* pod)
		{
			color(std::cout, _level);
			std::cout << pod;
			return *this;
		}

		m::system::ILogImpl& ColorConsole::operator<<(m::u64 pod)
		{
			color(std::cout, _level);
			std::cout << pod;
			return *this;
		}

		m::system::ILogImpl& ColorConsole::operator<<(m::u32 pod)
		{
			color(std::cout, _level);
			std::cout << pod;
			return *this;
		}

		m::system::ILogImpl& ColorConsole::operator<<(m::u16 pod)
		{
			color(std::cout, _level);
			std::cout << pod;
			return *this;
		}

		m::system::ILogImpl& ColorConsole::operator<<(m::u8 pod)
		{
			color(std::cout, _level);
			std::cout << pod;
			return *this;
		}

		m::system::ILogImpl& ColorConsole::operator<<(m::i64 pod)
		{
			color(std::cout, _level);
			std::cout << pod;
			return *this;
		}

		m::system::ILogImpl& ColorConsole::operator<<(m::i32 pod)
		{
			color(std::cout, _level);
			std::cout << pod;
			return *this;
		}

		m::system::ILogImpl& ColorConsole::operator<<(m::i16 pod)
		{
			color(std::cout, _level);
			std::cout << pod;
			return *this;
		}

		m::system::ILogImpl& ColorConsole::operator<<(m::i8 pod)
		{
			color(std::cout, _level);
			std::cout << pod;
			return *this;
		}

		m::system::ILogImpl& ColorConsole::operator<<(m::f64 pod)
		{
			color(std::cout, _level);
			std::cout << pod;
			return *this;
		}

		m::system::ILogImpl& ColorConsole::operator<<(m::f32 pod)
		{
			color(std::cout, _level);
			std::cout << pod;
			return *this;
		}

		m::system::ILogImpl& ColorConsole::operator<<(bool pod)
		{
			color(std::cout, _level);
			std::cout << pod;
			return *this;
		}
	}
}
