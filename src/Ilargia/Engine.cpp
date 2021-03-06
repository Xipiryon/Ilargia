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

#include <algorithm>
#include <fstream>

// ***  MUON    ***
#include <Muon/String.hpp>
#include <Muon/System/Time.hpp>

// ***  CORE    ***
#include "Ilargia/Engine.hpp"
#include "SharedLibrary.hpp"

// ***  TYPE    ***
#include "Ilargia/Type/Vector.hpp"
#include "Ilargia/Type/TexCoord.hpp"
#include "Ilargia/Type/Color.hpp"
#include "Ilargia/Type/Matrix.hpp"
#include "Ilargia/Type/Quaternion.hpp"

// ***  SYSTEM  ***
#include "Ilargia/System/ScriptDriver.hpp"

namespace ilg
{
	void registerStaticLibraryLoadFunction(const m::String& name, void* fptr)
	{
	}

	void registerStaticLibraryUnloadFunction(const m::String& name, void* fptr)
	{
	}

	void Engine::dispatchKeyCallback(void* windowHandle, int key, int scancode, int action, int modifier)
	{
		auto managerList = SharedLibrary::getInstance().getManagers();
		for (auto manager = managerList.begin(); manager != managerList.end(); ++manager)
		{
			(*manager).manager->onKeyCallback(windowHandle, key, scancode, action, modifier);
		}
	}

	Engine::Engine()
		: m_log("ENGINE")
		, m_running(false)
		, m_paused(false)
		, m_deltaTime(0.f)
		, m_programTime(0.f)
	{
	}

	int Engine::main(int argc, char** argv)
	{
		if (!init(argc, argv))
		{
			return -1;
		}
		run();
		close();

		return 0;
	}

	bool Engine::init(int argc, char** argv)
	{
		Engine& engine = getInstance();
		{
			std::string _argv = argv[0];
			size_t pos = _argv.rfind(m::PATH_SEPARATOR);
			if (pos == std::string::npos)
			{
				pos = _argv.size();
			}
			engine.m_programPath = (_argv.substr(0, pos) + m::PATH_SEPARATOR).c_str();
		}

		auto& metadb = m::meta::MetaDatabase::getInstance();
		auto& manager = manager::ManagerFactory::getInstance();
		auto& script = system::ScriptDriver::getInstance();
		auto& sharedLib = SharedLibrary::getInstance();
		sharedLib.forwardArg(argc, argv);

		// Registering core classes
		engine._registerCoreClass();
		engine._registerCoreComponentManager();

		//Load configuration file
		if (!engine._loadConfig())
		{
			return false;
		}

		//Default KeyValue Variables
		m::String app = "##No_Application_Name##";
		m::String ver = "##No_Version##";
		//keyValue.retrieve("APP_NAME", app);
		//keyValue.retrieve("VERSION", ver);

		// Ok, everything seems good
		engine.m_log(m::LOG_INFO) << "Initialized: " << app << " " << ver << m::endl;
		return true;
	}

	void Engine::close()
	{
		//Unload libraries
		SharedLibrary::getInstance().unloadLibraries();

		// Close every stream
		m::system::Log::close();

		// Remove every ILogImpl
		m::system::Log::clearLogImpl();
	}

	void Engine::toggle()
	{
		Engine& e = getInstance();
		pause(!e.m_paused);
	}

	void Engine::pause(bool pause)
	{
		Engine& e = getInstance();
		e.m_paused = pause;
		e.m_deltaTime = 0.f;
	}

	void Engine::stop()
	{
		getInstance().m_running = false;
	}

	void Engine::_run()
	{
		auto& managerList = SharedLibrary::getInstance().getManagers();

		//Check there is managers to update
		if (managerList.size() == 0)
		{
			getInstance().m_log(m::LOG_WARNING) << "No manager loaded, nothing to do: exiting!" << m::endl;
			getInstance().m_running = false;
			return;
		}

		//Modules update
		for (auto it = managerList.begin(); it != managerList.end(); ++it)
		{
			(*it).manager->onUpdate(m_deltaTime);
		}

		// Retrieve time information
		m::f32 dt = m_clock.now();
		if (!m_paused)
		{
			m_deltaTime = dt;
		}
		m_programTime += dt;
		m_clock.start();
	}

	void Engine::run()
	{
		Engine& engine = getInstance();

		m::i32 modSize = SharedLibrary::getInstance().getManagers().size();
		if (modSize == 0)
		{
			engine.m_log(m::LOG_WARNING) << "No Manager loaded, nothing to do: exiting!" << m::endl;
			return;
		}
		engine.m_running = true;

		//Main loop execution time start
		engine.m_clock.start();

		auto& managerList = SharedLibrary::getInstance().m_managers;

		//Sort manager depending on exec order
		std::sort(managerList.begin(), managerList.end(),
				  [](const CManagerPair l, const CManagerPair& r)
		{
			return l.manager->getUpdateOrder() < r.manager->getUpdateOrder();
		}
		);

		//onInit functions
		m::i32 count = 0;
		for (auto it = managerList.begin(); it != managerList.end(); ++it)
		{
			(*it).manager->onInit();
			++count;
		}

#if defined(ILARGIA_DEBUG)
		engine.m_log(m::LOG_INFO) << "Loaded Manager:" << m::endl;
		for (auto it = managerList.begin(); it != managerList.end(); ++it)
		{
			engine.m_log(m::LOG_INFO) << "\t" << it->manager->getManagerName() << m::endl;
		}
		engine.m_log(m::LOG_INFO) << "Total: " << managerList.size() << m::endl;
#endif
		//onUpdate functions
#ifdef MUON_PLATFORM_WEB
		emscripten_set_main_loop(_run, 0, m_running);
#else
		while (engine.m_running)
		{
			engine._run();
		}
#endif
		//onTerm functions
		for (auto it = managerList.begin(); it != managerList.end(); ++it)
		{
			(*it).manager->onTerm();
		}
	}

	bool Engine::isRunning()
	{
		return getInstance().m_running;
	}

	bool Engine::isPaused()
	{
		return getInstance().m_paused;
	}

	m::f32 Engine::getDeltaTime()
	{
		return getInstance().m_deltaTime;
	}

	m::f32 Engine::getProgramTime()
	{
		return getInstance().m_programTime;
	}

	const m::String& Engine::getProgramPath()
	{
		return getInstance().m_programPath;
	}
}
