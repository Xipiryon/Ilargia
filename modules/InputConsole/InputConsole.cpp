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

#include <iostream>
#include <thread>
#include "Ilargia/Engine.hpp"
#include "InputConsole.hpp"

namespace ilg
{
	namespace mod
	{
		InputConsole::InputConsole(const m::String& name, m::i32 updateOrder)
			: ISimpleManager(name, updateOrder)
			, m_running(true)
			, m_thread(NULL)
		{
			//Set default log to LOG_INFO
			getLog(m::LOG_INFO);
		}

		InputConsole::~InputConsole()
		{
			delete m_thread;
		}

		void InputConsole::_run()
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
			char buffer[256];
			m::String cmd;
			while (m_running)
			{
				getLog() << "<Waiting for console input> (type 'exit' to leave)" << m::endl;
				std::cin >> buffer;
				getLog() << "Handling input: \"" << buffer << "\"" << m::endl;

				cmd = buffer;
				if (cmd == "exit")
				{
					m_running = false;
				}
			}
		}

		bool InputConsole::onInit()
		{
			getLog() << "Starting InputConsole thread..." << m::endl;
			m_thread = new std::thread(&InputConsole::_run, this);
			getLog() << "... Thread started" << m::endl;
			return true;
		}

		bool InputConsole::onUpdate(m::f32 dt)
		{
			if (!m_running)
			{
				// Ask to close the engine if not _running
				Engine::stop();
			}
			return true;
		}

		bool InputConsole::onTerm()
		{
			if (m_thread->joinable())
			{
				getLog() << "Stopping thread ..." << m::endl;
				m_thread->join();
				getLog() << "... Thread stopped" << m::endl;
			}
			return true;
		}
	}
}
