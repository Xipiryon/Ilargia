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

#include <sys/stat.h>
#if defined(MUON_PLATFORM_WINDOWS)
#	include <winsock2.h>
// Because it's Posix, and doesn't exists on Windows
#	define STDIN_FILENO 0
#endif

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
			struct timeval timeout;
			timeout.tv_sec = 1;
			while (m_running)
			{
				/*
				getLog() << "<Waiting for console input> (type 'exit' to leave)" << m::endl;
				std::cin >> buffer;
				getLog() << "Handling input: \"" << buffer << "\"" << m::endl;

				cmd = buffer;
				if (cmd == "exit")
				{
				m_running = false;
				}
				// */
#if 0
				FD_ZERO(&m_fd);
				FD_SET(STDIN_FILENO, &m_fd);

				m::i32 result = select(0, (fd_set*)&m_fd, NULL, NULL, &timeout);
				if (result == -1 && errno != EINTR)
				{
					//cerr << "Error in select: " << strerror(errno) << "\n";
					break;
				}
				else if (result == -1 && errno == EINTR)
				{
					//we've received and interrupt - handle this
				}
				else
				{
					if (FD_ISSET(STDIN_FILENO, &m_fd))
					{
						cmd = buffer;
					}
				}
#endif
			}
		}

		void InputConsole::onInit()
		{
			getLog() << "Starting InputConsole thread..." << m::endl;
			m_thread = new std::thread(&InputConsole::_run, this);
			getLog() << "... Thread started" << m::endl;
		}

		void InputConsole::onUpdate(m::f32 dt)
		{
			if (!m_running)
			{
				// Ask to close the engine if not _running
				Engine::stop();
			}
		}

		void InputConsole::onTerm()
		{
			if (m_thread->joinable())
			{
				m_running = false;
				getLog() << "Stopping thread ..." << m::endl;
				m_thread->join();
				getLog() << "... Thread stopped" << m::endl;
			}
		}
	}
}
