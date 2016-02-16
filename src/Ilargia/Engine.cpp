/*************************************************************************
* Ilargia Engine - http://github.com/Xleek/Ilargia
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
	void Engine::dispatchKeyCallback(void* windowHandle, int key, int scancode, int action, int modifier)
	{
		auto managerList = SharedLibrary::getInstance().getManagers();
		for(auto manager = managerList.begin(); manager != managerList.end(); ++manager)
		{
			(*manager).manager->onKeyCallback(windowHandle, key, scancode, action, modifier);
		}
	}

	Engine::Engine()
		: _log("ENGINE")
		, _running(false)
		, _paused(false)
		, _deltaTime(0.f)
		, _programTime(0.f)
	{
	}

	int Engine::main(int argc, char** argv)
	{
		if(!init(argc, argv))
		{
			return -1;
		}
		run();
		close();

		return 0;
	}

	bool Engine::init(int argc, char** argv)
	{
#if defined(MUON_DEBUG)
		muon::system::Log::registerDefaultLogImpl();
#endif

		Engine& engine = createInstance();
		{
			std::string _argv = argv[0];
			size_t pos = _argv.rfind(muon::PATH_SEPARATOR);
			if(pos == std::string::npos)
			{
				pos = _argv.size();
			}
			engine._programPath = (_argv.substr(0, pos) + muon::PATH_SEPARATOR).c_str();
		}

		//auto& keyValue = muon::system::KeyValue::getInstance();
		auto& script = system::ScriptDriver::createInstance();
		auto& sharedLib = SharedLibrary::createInstance();
		sharedLib.forwardArg(argc, argv);

#if defined(MUON_DEBUG)
		//keyValue.store("MUON_DEBUG", true);
#else
		//keyValue.store("MUON_DEBUG", false);
#endif

		// Registering core AS classes
		if(!engine._registerClasses())
		{
			return false;
		}

		//Load configuration file
		if(!engine._loadConfig())
		{
			return false;
		}

		//Default KeyValue Variables
		muon::String app = "##No_Application_Name##";
		muon::String ver = "##No_Version##";
		//keyValue.retrieve("APP_NAME", app);
		//keyValue.retrieve("VERSION", ver);

		// Ok, everything seems good
		engine._log(muon::LOG_INFO) << "Initialized: " << app << " " << ver << muon::endl;
		return true;
	}

	void Engine::close()
	{
		//Unload libraries
		SharedLibrary::getInstance().unloadLibraries();

		// Close every stream
		muon::system::Log::close();

		// Remove every ILogImpl
		muon::system::Log::clearLogImpl();
	}

	void Engine::toggle()
	{
		Engine& e = getInstance();
		pause(!e._paused);
	}

	void Engine::pause(bool pause)
	{
		Engine& e = getInstance();
		e._paused = pause;
		e._deltaTime = 0.f;
	}

	void Engine::stop()
	{
		getInstance()._running = false;
	}

	void Engine::_run()
	{
		auto& managerList = SharedLibrary::getInstance().getManagers();

		//Check there is managers to update
		if (managerList.size() == 0)
		{
			getInstance()._log(muon::LOG_WARNING) << "No manager loaded, nothing to do: exiting!" << muon::endl;
			getInstance()._running = false;
			return;
		}

		//Modules update
		for(auto it = managerList.begin(); it != managerList.end(); ++it)
		{
			bool success = (*it).manager->onUpdate(_deltaTime);
			MUON_ASSERT(success, "Module: \"%s\" onUpdate() failed!", (*it).manager->getManagerName().cStr());
		}

		// Retrieve time information
		muon::f32 dt = _clock.now();
		if (!_paused)
		{
			_deltaTime = dt;
		}
		_programTime += dt;
		_clock.start();
	}

	void Engine::run()
	{
		Engine& engine = getInstance();

		muon::i32 modSize = SharedLibrary::getInstance().getManagers().size();
		if (modSize == 0)
		{
			engine._log(muon::LOG_WARNING) << "No manager loaded, nothing to do: exiting!" << muon::endl;
			return;
		}
		else
		{
			engine._running = true;
			engine._log(muon::LOG_INFO) << "Running with " << modSize << " IComponentManager instance(s) loaded!" << muon::endl;
		}

		//Main loop execution time start
		engine._clock.start();

		auto& managerList = SharedLibrary::getInstance()._managers;

		//Sort manager depending on exec order
		std::sort(managerList.begin(), managerList.end(),
			[](const CManagerPair l, const CManagerPair& r)
			{
				return l.manager->getUpdateOrder() < r.manager->getUpdateOrder();
			}
		);

		//onInit functions
		std::vector<muon::i32> failed;
		failed.reserve(modSize);
		muon::i32 count = 0;
		for(auto it = managerList.begin(); it != managerList.end(); ++it)
		{
			bool success = (*it).manager->onInit();
			MUON_ASSERT(success, "Module: \"%s\" onInit() failed!", (*it).manager->getManagerName().cStr());
			if (!success)
			{
				failed.push_back(count);
				engine._log(muon::LOG_WARNING) << "Module: \"" << (*it).manager->getManagerName() << "\" onInit() failed!" << muon::endl;
			}
			++count;
		}

		//Remove all "onInit" fail manager
		while(!failed.empty())
		{
			muon::i32 i = failed.back();
			IComponentManager* manager = managerList[i].manager;
			engine._log(muon::LOG_WARNING) << "Removing \"" << manager->getManagerName() << "\" due to onInit() failure. Calling onTerm() ..." << muon::endl;

			bool success = manager->onTerm();
			MUON_ASSERT(success, "Module: \"%s\" onTerm() failed!", manager->getManagerName().cStr());
			MUON_DELETE(manager);
			managerList.erase(managerList.begin() + i);
			failed.pop_back();
		}

		//onUpdte fonctions
#ifdef EMSCRIPTEN
		emscripten_set_main_loop(_run, 0, _running);
#else
		while(engine._running)
		{
			engine._run();
		}
#endif
		//onTerm functions
		for(auto it = managerList.begin(); it != managerList.end(); ++it)
		{
			bool success = (*it).manager->onTerm();
			MUON_ASSERT(success, "Module: \"%s\" onTerm() failed!", (*it).manager->getManagerName().cStr());
		}
	}

	bool Engine::isRunning()
	{
		return getInstance()._running;
	}

	bool Engine::isPaused()
	{
		return getInstance()._paused;
	}

	muon::f32 Engine::getDeltaTime()
	{
		return getInstance()._deltaTime;
	}

	muon::f32 Engine::getProgramTime()
	{
		return getInstance()._programTime;
	}

	const char* Engine::getProgramPath()
	{
		return getInstance()._programPath.cStr();
	}
}
