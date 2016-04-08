/*************************************************************************
* Ilargia Engine - http://github.com/Xleek/Ilargia
* Data oriented OpenGL game engine, mostly for learning purpose
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

#ifndef INCLUDE_ILARGIA_INPUTCONSOLEMODULE_HPP
#define INCLUDE_ILARGIA_INPUTCONSOLEMODULE_HPP

#include "Ilargia/Manager/ISimpleManager.hpp"
namespace std
{
	class thread;
}

namespace ilg
{
	namespace mod
	{
		class ILARGIA_API InputConsole : public ilg::manager::ISimpleManager
		{
		public:
			InputConsole(const m::String& name, m::i32 updateOrder);
			virtual ~InputConsole();

			virtual void onInit();
			virtual void onUpdate(m::f32);
			virtual void onTerm();

		private:
			void _run();
			bool m_running;
			std::thread* m_thread;
		};
	}
}

#endif
