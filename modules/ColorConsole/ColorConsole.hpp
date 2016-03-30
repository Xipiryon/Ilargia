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

#ifndef INCLUDE_ILARGIA_COLORCONSOLE_HPP
#define INCLUDE_ILARGIA_COLORCONSOLE_HPP

#include <Muon/System/Log.hpp>
#include <Ilargia/Engine.hpp>

namespace ilg
{
	namespace mod
	{
		class ILARGIA_API ColorConsole : public m::system::ILogImpl
		{
		public:
			ColorConsole();
			virtual ~ColorConsole();

			virtual m::system::ILogImpl& endl();
			virtual void operator()(m::LogLevel level);

			virtual m::system::ILogImpl& operator<<(const char* pod);
			virtual m::system::ILogImpl& operator<<(m::u64 pod);
			virtual m::system::ILogImpl& operator<<(m::u32 pod);
			virtual m::system::ILogImpl& operator<<(m::u16 pod);
			virtual m::system::ILogImpl& operator<<(m::u8 pod);
			virtual m::system::ILogImpl& operator<<(m::i64 pod);
			virtual m::system::ILogImpl& operator<<(m::i32 pod);
			virtual m::system::ILogImpl& operator<<(m::i16 pod);
			virtual m::system::ILogImpl& operator<<(m::i8 pod);
			virtual m::system::ILogImpl& operator<<(m::f64 pod);
			virtual m::system::ILogImpl& operator<<(m::f32 pod);
			virtual m::system::ILogImpl& operator<<(bool pod);

		protected:
			m::LogLevel _level;
		};
	}
}

#endif
