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

#ifndef _ILARGIA_COLORCONSOLE_H_INCLUDED
#define _ILARGIA_COLORCONSOLE_H_INCLUDED

#include <Muon/System/Log.hpp>
#include <Ilargia/Engine.hpp>

namespace ilg
{
	namespace mod
	{
		class ILG_API ColorConsole : public muon::system::ILogImpl
			{
			public:
				ColorConsole();
				virtual ~ColorConsole();

				virtual muon::system::ILogImpl& endl(muon::system::ILogImpl& stream);
				virtual void operator()(muon::LogLevel level);

				virtual muon::system::ILogImpl& operator<<(const char* pod);
				virtual muon::system::ILogImpl& operator<<(muon::u64 pod);
				virtual muon::system::ILogImpl& operator<<(muon::u32 pod);
				virtual muon::system::ILogImpl& operator<<(muon::u16 pod);
				virtual muon::system::ILogImpl& operator<<(muon::u8 pod);
				virtual muon::system::ILogImpl& operator<<(muon::i64 pod);
				virtual muon::system::ILogImpl& operator<<(muon::i32 pod);
				virtual muon::system::ILogImpl& operator<<(muon::i16 pod);
				virtual muon::system::ILogImpl& operator<<(muon::i8 pod);
				virtual muon::system::ILogImpl& operator<<(muon::f64 pod);
				virtual muon::system::ILogImpl& operator<<(muon::f32 pod);
				virtual muon::system::ILogImpl& operator<<(bool pod);

			protected:
				muon::LogLevel _level;
		};
	}
}

#endif
