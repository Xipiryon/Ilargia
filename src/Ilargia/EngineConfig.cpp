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

#include <Muon/String.hpp>
#include "Ilargia/Engine.hpp"
#include "SharedLibrary.hpp"

#include "Ilargia/System/tinyxml2.h"

namespace ilg
{
	bool Engine::_loadConfig()
	{
		std::function<void(muon::String&)> toUpper = [](muon::String& s)
		{
			for(muon::u32 i = 0; i < s.size(); ++i)
			{
				//Uppercase
				s[i] = (s[i] >= 'a' && s[i] <= 'z' ? s[i] - ((int)'a'-'A') : s[i]);
			}
		};

		using namespace tinyxml2;
		XMLDocument doc;
		muon::String configFile = getProgramPath();
		configFile += "config.xml";

		if(doc.LoadFile(configFile.cStr()) != XML_SUCCESS)
		{
			_log(muon::LOG_ERROR) << "Couldn't open \"" << configFile << "\": Error code: " << doc.ErrorName() << muon::endl;
			return false;
		}

		XMLElement* root = doc.RootElement();
		if(!(root && strcmp(root->Name(), "Engine") == 0))
		{
			_log(muon::LOG_ERROR) << "No root element in \"" << configFile << "\", or root is different from \"Engine\"" << muon::endl;
			return false;
		}

		/*******************************/
		/**********    LOGS   **********/
		/*******************************/
		const XMLElement* logs = root->FirstChildElement("Logs");
		muon::String logOutput = "output_log";
		muon::LogLevel logLevel = muon::LOG_INFO;
		if(logs)
		{
			const XMLAttribute* attrOutput = logs->FindAttribute("Output");
			if(attrOutput)
			{
				logOutput = attrOutput->Value();
				_log(muon::LOG_INFO) << "Log Output value set to \"" << logOutput << "\"" << muon::endl;
			}

			const XMLAttribute* attrLevel = logs->FindAttribute("Level");
			if(attrLevel)
			{
				muon::String level = attrLevel->Value();
				toUpper(level);

				if(level == "DEBUG") { logLevel = muon::LOG_DEBUG; }
				else if(level == "WARNING") { logLevel = muon::LOG_WARNING; }
				else if(level == "ERROR") { logLevel = muon::LOG_ERROR; }
				else
				{
					level = "INFO";
					logLevel = muon::LOG_INFO;
				}
				_log(muon::LOG_INFO) << "Log Level set to \"" << level << "\"" << muon::endl;
			}

			const XMLAttribute* attrDefaultLogImpl = logs->FindAttribute("EnableDefaultLogImpl");
			if(attrDefaultLogImpl)
			{
				bool enableDefaultLogImpl = attrDefaultLogImpl->BoolValue();
				if(enableDefaultLogImpl)
				{
					muon::system::Log::registerDefaultLogImpl();
				}
			}

		}
		else
		{
			//No Logs: use default value
			_log(muon::LOG_WARNING) << "No Logs settings: using default (ouput = \"output_log\", level=\"INFO\")" << muon::endl;
		}
		muon::system::Log::open(logOutput);
		muon::system::Log::setLevel(logLevel);

		/*******************************/
		/**********  MODULES  **********/
		/*******************************/
		const XMLElement* modules = root->FirstChildElement("Modules");
		if(modules)
		{
			/*
			* TODO
			Check Static / Dynamic attribute
			and forward it to SharedLibrary class
			*/
			const XMLAttribute* attrLink = modules->FindAttribute("Link");
			if(attrLink)
			{
				muon::String link = attrLink->Value();
				toUpper(link);
				if(link == "STATIC")
				{
					SharedLibrary::getInstance().setLinkStatic();
				}
			}
			muon::String path;
			const XMLAttribute* attrPath = modules->FindAttribute("Path");
			if(!attrPath)
			{
				_log(muon::LOG_WARNING) << "No \"Path\" attribute in \"Modules\" node!" << muon::endl;
			}
			else if (strcmp(attrPath->Value(), ".") != 0)
			{
				path = attrPath->Value();
				path += muon::PATH_SEPARATOR;
			}

			XMLElement* module = (XMLElement*)modules->FirstChildElement("Module");
			if(!module)
			{
				_log(muon::LOG_ERROR) << "No \"Module\" elements! You should add some, don't you think?" << muon::endl;
				return false;
			}

			while(module)
			{
				const XMLElement* mod = (const XMLElement*)module;
				if(strcmp(mod->Name(), "Module") == 0)
				{
					const XMLAttribute* attrFile = mod->FindAttribute("File");
					muon::String file;

					if(attrFile) { file = attrFile->Value(); }
					else
					{
						_log(muon::LOG_ERROR) << "Module doesn't have \"LibraryFile\" attribute!" << muon::endl;
						return false;
					}


#if defined(ILARGIA_DEBUG)
					muon::String suffix = "-d";
#else
					muon::String suffix;
#endif

#if defined(MUON_PLATFORM_WINDOWS)
					muon::String filePath = path+file+suffix+".dll";
#else
					muon::String filePath = path+"lib"+file+suffix+".so";
#endif
					SharedLibrary::getInstance().loadLibrary(file, filePath);
				}
				module = (XMLElement*)mod->NextSibling();
			}
		}
		else
		{
			_log(muon::LOG_WARNING) << "No \"Modules\" elements! You should add some, don't you think?" << muon::endl;
		}

		/*******************************/
		/********** KEYVALUES **********/
		/*******************************/
		const XMLElement* keyValues = root->FirstChildElement("KeyValues");
		if(keyValues)
		{
			XMLElement* keyValue = (XMLElement*)keyValues->FirstChildElement("KeyValue");
			while(keyValue)
			{
				const XMLElement* kv = (const XMLElement*)keyValue;
				if(strcmp(kv->Name(), "KeyValue") == 0)
				{
					const XMLAttribute* attrName = kv->FindAttribute("Name");
					const XMLAttribute* attrValue = kv->FindAttribute("Value");
					const XMLAttribute* attrType = kv->FindAttribute("Type");
					muon::String name;
					muon::String value;
					muon::String type;

					if(!attrName || !attrValue || !attrType)
					{
						if(!attrName) _log(muon::LOG_ERROR) << "KeyValue doesn't have a \"Name\" attribute" << muon::endl;
						if(!attrValue) _log(muon::LOG_ERROR) << "KeyValue doesn't have a \"Value\" attribute" << muon::endl;
						if(!attrType) _log(muon::LOG_ERROR) << "KeyValue doesn't have a \"Type\" attribute" << muon::endl;
					}
					else
					{
						name = attrName->Value();
						type = attrType->Value();
						toUpper(type);
						//TODO Fix It
						/*
						muon::system::KeyValue& kvs = muon::system::KeyValue::getInstance();

						if(type == "INT") {kvs.store(name, attrValue->IntValue());}
						else if(type == "FLOAT") {kvs.store(name, attrValue->FloatValue());}
						else if(type == "BOOL") {kvs.store(name, attrValue->BoolValue());}
						else if(type == "STRING") {kvs.store(name, attrValue->Value());}
						else
						{
							if(!attrType) _log(muon::LOG_ERROR) << "KeyValue \"" << name << "\" Type is incorrect (\"" << type << "\")" << muon::endl;
						}
						//*/
					}
				}

				keyValue = (XMLElement*)kv->NextSibling();
			}
		}
		return true;
	}
}
