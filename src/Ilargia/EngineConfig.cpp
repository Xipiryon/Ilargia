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
#include <map>
#include <sys/types.h>
#include <dirent.h>

#include <Muon/String.hpp>
#include <picojson.h>
#include "Ilargia/Engine.hpp"
#include "SharedLibrary.hpp"

namespace ilg
{
	bool Engine::_loadConfig()
	{
		m::String configFilename = getProgramPath() + "config.json";

		std::function<void(m::String&)> toUpper = [](m::String& s)
		{
			for (m::u32 i = 0; i < s.size(); ++i)
			{
				//Uppercase
				s[i] = (s[i] >= 'a' && s[i] <= 'z' ? s[i] - ((int)'a' - 'A') : s[i]);
			}
		};

		picojson::value v;
		std::ifstream config(configFilename.cStr());
		config >> v;
		m::String err = picojson::get_last_error().c_str();
		if (!err.empty())
		{
			m_log(m::LOG_ERROR) << "Couldn't open \"" << configFilename << "\": " << err << m::endl;
			return false;
		}

		// We must have an object as root
		if (!v.is<picojson::object>())
		{
			m_log(m::LOG_ERROR) << "\"" << configFilename << "\" root element is not an object!" << m::endl;
			return false;
		}

		// Log init variable
		m::String logOutput = "engine";
		m::LogLevel logLevel = m::LOG_ERROR;
		// Modules init variable
		struct ModuleIncExc
		{
			m::String path;
			bool include = true;
		};
		std::map<m::String, ModuleIncExc> moduleIncludeExclude;

		// Look for "Engine" object, which contains our value
		auto& root = v.get<picojson::object>();
		for (auto itCategory = root.begin(); itCategory != root.end(); ++itCategory)
		{
			// Found "Engine" category, extract sub element
			if (itCategory->first == "Engine")
			{
				auto& engine = itCategory->second.get<picojson::object>();
				for (auto itConfig = engine.begin(); itConfig != engine.end(); ++itConfig)
				{
					// LOG
					// ***********
					if (itConfig->first == "Log")
					{
						auto& log = itConfig->second.get<picojson::object>();
						auto it = log.end();
						// Output
						it = log.find("Filename");
						if (it != log.end() && it->second.is<std::string>())
						{
							logOutput = it->second.get<std::string>().c_str();
						}

						// Level
						it = log.find("Level");
						if (it != log.end() && it->second.is<std::string>())
						{
							std::string logLevelStr;
							logLevelStr = it->second.get<std::string>();
							if (logLevelStr == "DEBUG")
							{
								logLevel = m::LOG_DEBUG;
							}
							else if (logLevelStr == "WARNING")
							{
								logLevel = m::LOG_WARNING;
							}
							else if (logLevelStr == "ERROR")
							{
								logLevel = m::LOG_ERROR;
							}
							else if (logLevelStr == "INFO")
							{
								logLevel = m::LOG_INFO;
							}
						}

						// EnableDefaultLogImpl
						it = log.find("EnableDefaultLogImpl");
						if (it != log.end() && it->second.is<bool>())
						{
							if (it->second.get<bool>())
							{
								m::system::Log::registerDefaultLogImpl();
							}
						}
					}
					// MODULES
					// ***********
					else if (itConfig->first == "Modules")
					{
						auto& modules = itConfig->second.get<picojson::object>();
						auto it = modules.end();

						// List
						it = modules.find("List");
						if (it != modules.end() && it->second.is<picojson::array>())
						{
							auto& incList = it->second.get<picojson::array>();
							for (auto itMod : incList)
							{
								if (!itMod.is<picojson::object>())
								{
									continue;
								}

								auto& mod = itMod.get<picojson::object>();
								auto itName = mod.find("Name");
								auto itPath = mod.find("Path");
								auto itLoad = mod.find("Load");
								if (itName != mod.end() && itName->second.is<std::string>()
									&& itPath != mod.end() && itPath->second.is<std::string>()
									&& itLoad != mod.end() && itLoad->second.is<bool>())
								{
									ModuleIncExc mod;
									mod.path = itPath->second.get<std::string>().c_str();
									mod.include &= itLoad->second.get<bool>();
									moduleIncludeExclude[itName->second.get<std::string>().c_str()] = mod;
								}
							}
						}
					}
				}
			}
		}
		m::system::Log::open(logOutput);
		m::system::Log::setLevel(logLevel);

		for (auto mod : moduleIncludeExclude)
		{
			if (mod.second.include)
			{
#if defined(ILARGIA_STATIC)
				SharedLibrary::getInstance().loadLibrary(mod.first, "");
#else
				m::String filename;
#	if defined(MUON_PLATFORM_WINDOWS)
				filename = mod.first + ".dll";
#	else
				filename = "lib" + mod.first + ".so";
#	endif
				m::String filepath = getProgramPath() + mod.second.path + m::PATH_SEPARATOR + filename;
				SharedLibrary::getInstance().loadLibrary(mod.first, filepath);
#endif
			}
		}
		return true;
	}
}
