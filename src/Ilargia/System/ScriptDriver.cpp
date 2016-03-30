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

#include <cstdio>
#include <Muon/System/Assert.hpp>
#include "Ilargia/Engine.hpp"
#include "Ilargia/System/ScriptDriver.hpp"

namespace
{
	static const m::String defaultModuleName = "Global";
}

namespace ilg
{
	namespace system
	{
		void ScriptDriver::check(int r)
		{
			MUON_ASSERT_BREAK(r >= 0, "Check has returned the invalid error code: %d\n", r);
		}

		ScriptDriver::ScriptDriver()
			: _log("ScriptDriver")
			, _engine(NULL)
			, _context(NULL)
			, _moduleCompiled(new std::unordered_map<m::String, bool>)
		{
			//_engine->SetMessageCallback(asMETHOD(Script, errorCallback), this, asCALL_THISCALL);
		}

		ScriptDriver::~ScriptDriver()
		{
			//_context->Release();
			//_engine->Release();
			delete _moduleCompiled;
		}

		void ScriptDriver::_errorCallback(const m::String& msg)
		{
			/*
			LogLevel l;
			switch(msg->type)
			{
			case asMSGTYPE_ERROR:
			l = LOG_ERROR;
			break;
			case asMSGTYPE_WARNING:
			l = LOG_WARNING;
			break;
			case asMSGTYPE_INFORMATION:
			l = LOG_INFO;
			break;
			default:
			l = LOG_DEBUG;
			break;
			};
			_log(l) << "In section \n\t>'" << msg->section
			<< "' (line " << msg->row
			<< "):\n\t>'" << msg->message << "'" << endl;
			//*/
		}

		IScriptEngine* ScriptDriver::getScriptEngine() const
		{
			return _engine;
		}

		IScriptContext* ScriptDriver::getScriptContext() const
		{
			return _context;
		}

		ScriptState ScriptDriver::load(const m::String& filename)
		{
			return load(filename, defaultModuleName);
		}

		ScriptState ScriptDriver::load(const m::String& filename, const m::String& moduleName)
		{
			(*_moduleCompiled)[moduleName] = false;
			//_context->Unprepare();

			MUON_ASSERT(!filename.empty(), "Failed to open file: no filename given!");
			if (filename.empty())
			{
				_log(m::LOG_ERROR) << "Failed to open script file: no filename given!";
				return SCRIPT_FAILED_OPEN;
			}

			m::String completePath;
			if (filename[0] != m::PATH_SEPARATOR)
			{
				completePath = Engine::getProgramPath() + filename;
			}
			else
			{
				completePath = filename;
			}

			ScriptState sls = _load(completePath, moduleName);
			switch (sls)
			{
				case SCRIPT_FAILED_OPEN:
					_log(m::LOG_ERROR) << "Script file \"" << completePath << "\" not found" << m::endl;
					break;
				case SCRIPT_FAILED_LOAD:
					_log(m::LOG_ERROR) << "Script file \"" << completePath << "\" couldn't be loaded" << m::endl;
					break;
				case SCRIPT_FAILED_SECTION:
					_log(m::LOG_ERROR) << "Script file \"" << completePath << "\" couldn't be added in Script Engine correctly!" << m::endl;
					break;
				case SCRIPT_FAILED_BUILD:
					_log(m::LOG_ERROR) << "Script file \"" << completePath << "\" couldn't be build." << m::endl;
					_log(m::LOG_ERROR) << "\t>Please check syntax errors or use of unbinded classes" << m::endl;
					break;
				case SCRIPT_SUCCESS:
				{
					m::String moduleNameLog = (moduleName.empty() ? "" : " (Module: " + moduleName + ") ");
					_log(m::LOG_INFO) << "Script \"" << filename << "\"" << moduleNameLog << " is correctly loaded!" << m::endl;
				}
				break;
			}
			return sls;
		}

		ScriptState ScriptDriver::_load(const m::String& filename, const m::String& moduleName)
		{
			/*
			_log(m::LOG_DEBUG) << "Loading file: \"" << filename << "\" | module: \"" << moduleName << "\"" << endl;
			int r;
			FILE* file = fopen(filename.cStr(), "rb");
			if(file == 0)
			{
			return SCRIPT_FAILED_OPEN;
			}

			fseek(file, 0, SEEK_END);
			unsigned int len = (unsigned int)ftell(file);
			fseek(file, 0, SEEK_SET);

			char* script = (char*)malloc(sizeof(char) * len + 1);
			r = fread(script, len, 1, file);
			fclose(file);
			if(r == 0)
			{
			free(script);
			return SCRIPT_FAILED_OPEN;
			}
			script[len] = 0;

			IScriptModule* mod = _engine->GetModule(moduleName.cStr(), asGM_CREATE_IF_NOT_EXISTS);
			r = mod->AddScriptSection(filename.cStr(), script, len);
			free(script);
			if( r < 0 )
			{
			return SCRIPT_FAILED_SECTION;
			}
			//*/
			return SCRIPT_SUCCESS;
		}

		ScriptState ScriptDriver::compile()
		{
			return compile(defaultModuleName);
		}

		ScriptState ScriptDriver::compile(const m::String& moduleName)
		{
			/*
			IScriptModule* mod = _engine->GetModule(moduleName.cStr(), asGM_ONLY_IF_EXISTS);
			if(!mod)
			{
			_log(m::LOG_ERROR) << "No module \"" << moduleName << "\" found!" << endl;
			return SCRIPT_FAILED_BUILD;
			}

			if(mod->Build() < 0)
			{
			_log(m::LOG_ERROR) << "Failed to build the module \"" << moduleName << "\"!" << endl;
			return SCRIPT_FAILED_BUILD;
			}
			(*_moduleCompiled)[moduleName] = true;
			//*/
			return SCRIPT_SUCCESS;
		}

		ScriptState ScriptDriver::prepare(const m::String& funcName, IScriptContext** ctx)
		{
			return prepare(funcName, defaultModuleName, ctx);
		}

		ScriptState ScriptDriver::prepare(const m::String& funcName, const m::String& moduleName, IScriptContext** ctx)
		{
			/*
			if(_moduleCompiled->find(moduleName) == _moduleCompiled->end() || (*_moduleCompiled)[moduleName] == false)
			{
			_log(m::LOG_ERROR) << "Module \"" << moduleName << "\" has not been compiled, cannot execute!" << endl;
			return SCRIPT_FAILED_PREPARE;
			}

			_context->Unprepare();
			IScriptFunction* func = _engine->GetModule(moduleName.cStr())->GetFunctionByDecl(funcName.cStr());
			if(func == 0)
			{
			// The function couldn't be found. Instruct the script writer
			// to include the expected function in the script.
			_log(m::LOG_ERROR) << "The script must have the function '" << funcName << "'. Please add it and try again." << endl;
			return SCRIPT_FAILED_LOAD;
			}
			else
			{
			int r = _context->Prepare(func);
			if(r != 0)
			{
			return SCRIPT_FAILED_PREPARE;
			}
			}
			if (ctx != NULL)
			{
			*ctx = _context;
			}
			//*/
			return SCRIPT_SUCCESS;
		}

		ScriptState ScriptDriver::execute()
		{
			/*
			int r = _context->Execute();
			if (r != asEXECUTION_FINISHED)
			{
			// The execution didn't complete as expected. Determine what happened.
			if (r == asEXECUTION_EXCEPTION)
			{
			// An exception occurred, let the script writer know what happened so it can be corrected.
			_log(m::LOG_ERROR) << "An exception '%s' occurred. "
			<< "Please correct the code and try again:\n\t>"
			<< _context->GetExceptionString()
			<< endl;
			return SCRIPT_FAILED_EXECUTE;
			}
			}
			//*/
			return SCRIPT_SUCCESS;
		}

		bool ScriptDriver::eval(const m::String& script)
		{
			return true;
		}

		bool ScriptDriver::function(const m::String& name)
		{
			return true;
		}
	}
}
