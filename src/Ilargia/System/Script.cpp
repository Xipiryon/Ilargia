/*************************************************************************
* DNA Engine - http://gitweb.louis-schnellbach.com
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
#include "Ilargia/Engine.hpp"
#include "Ilargia/System/Script.hpp"

namespace
{
	static const muon::String defaultModuleName = "Global";
}

namespace ilg
{
	namespace system
	{
		void Script::check(int r)
		{
			MUON_ASSERT_BREAK(r >= 0, "Check has returned the invalid error code: %d\n", r);
		}

		Script::Script()
			: _log("AngelScript")
			//, _engine(asCreateScriptEngine(ANGELSCRIPT_VERSION))
			//, _context(_engine->CreateContext())
			, _moduleCompiled(new std::unordered_map<muon::String, bool>)
		{
			//_engine->SetMessageCallback(asMETHOD(Script, errorCallback), this, asCALL_THISCALL);
		}

		Script::~Script()
		{
			//_context->Release();
			//_engine->Release();
			delete _moduleCompiled;
		}

		void Script::errorCallback(const asSMessageInfo* msg)
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

		asIScriptEngine* Script::getScriptEngine() const
		{
			return _engine;
		}

		asIScriptContext* Script::getScriptContext() const
		{
			return _context;
		}

		ScriptState Script::load(const muon::String& filename)
		{
			return load(filename, defaultModuleName);
		}

		ScriptState Script::load(const muon::String& filename, const muon::String& moduleName)
		{
			(*_moduleCompiled)[moduleName] = false;
			//_context->Unprepare();

			MUON_ASSERT(!filename.empty(), "Failed to open file: no filename given!");
			if (filename.empty())
			{
				_log(muon::LOG_ERROR) << "Failed to open script file: no filename given!";
				return SCRIPT_FAILED_OPEN;
			}

			muon::String completePath;
			if (filename[0] != muon::PATH_SEPARATOR)
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
				_log(muon::LOG_ERROR) << "Script file \"" << completePath << "\" not found" << muon::endl;
				break;
			case SCRIPT_FAILED_LOAD:
				_log(muon::LOG_ERROR) << "Script file \"" << completePath << "\" couldn't be loaded" << muon::endl;
				break;
			case SCRIPT_FAILED_SECTION:
				_log(muon::LOG_ERROR) << "Script file \"" << completePath << "\" couldn't be added in Script Engine correctly!" << muon::endl;
				break;
			case SCRIPT_FAILED_BUILD:
				_log(muon::LOG_ERROR) << "Script file \"" << completePath << "\" couldn't be build." << muon::endl;
				_log(muon::LOG_ERROR) << "\t>Please check syntax errors or use of unbinded classes" << muon::endl;
				break;
			case SCRIPT_SUCCESS:
				{
					muon::String moduleNameLog = (moduleName.empty() ? "" : " (Module: " + moduleName + ") ");
					_log(muon::LOG_INFO) << "Script \"" << filename << "\"" << moduleNameLog <<" is correctly loaded!" << muon::endl;
				}
				break;
			}
			return sls;
		}

		ScriptState Script::_load(const muon::String& filename, const muon::String& moduleName)
		{
			/*
			_log(muon::LOG_DEBUG) << "Loading file: \"" << filename << "\" | module: \"" << moduleName << "\"" << endl;
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

			asIScriptModule* mod = _engine->GetModule(moduleName.cStr(), asGM_CREATE_IF_NOT_EXISTS);
			r = mod->AddScriptSection(filename.cStr(), script, len);
			free(script);
			if( r < 0 )
			{
				return SCRIPT_FAILED_SECTION;
			}
			//*/
			return SCRIPT_SUCCESS;
		}

		ScriptState Script::compile()
		{
			return compile(defaultModuleName);
		}

		ScriptState Script::compile(const muon::String& moduleName)
		{
			/*
			asIScriptModule* mod = _engine->GetModule(moduleName.cStr(), asGM_ONLY_IF_EXISTS);
			if(!mod)
			{
				_log(muon::LOG_ERROR) << "No module \"" << moduleName << "\" found!" << endl;
				return SCRIPT_FAILED_BUILD;
			}

			if(mod->Build() < 0)
			{
				_log(muon::LOG_ERROR) << "Failed to build the module \"" << moduleName << "\"!" << endl;
				return SCRIPT_FAILED_BUILD;
			}
			(*_moduleCompiled)[moduleName] = true;
			//*/
			return SCRIPT_SUCCESS;
		}

		ScriptState Script::prepare(const muon::String& funcName, asIScriptContext** ctx)
		{
			return prepare(funcName, defaultModuleName, ctx);
		}

		ScriptState Script::prepare(const muon::String& funcName, const muon::String& moduleName, asIScriptContext** ctx)
		{
			/*
			if(_moduleCompiled->find(moduleName) == _moduleCompiled->end() || (*_moduleCompiled)[moduleName] == false)
			{
				_log(muon::LOG_ERROR) << "Module \"" << moduleName << "\" has not been compiled, cannot execute!" << endl;
				return SCRIPT_FAILED_PREPARE;
			}

			_context->Unprepare();
			asIScriptFunction* func = _engine->GetModule(moduleName.cStr())->GetFunctionByDecl(funcName.cStr());
			if(func == 0)
			{
				// The function couldn't be found. Instruct the script writer
				// to include the expected function in the script.
				_log(muon::LOG_ERROR) << "The script must have the function '" << funcName << "'. Please add it and try again." << endl;
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

		ScriptState Script::execute()
		{
			/*
			int r = _context->Execute();
			if (r != asEXECUTION_FINISHED)
			{
				// The execution didn't complete as expected. Determine what happened.
				if (r == asEXECUTION_EXCEPTION)
				{
					// An exception occurred, let the script writer know what happened so it can be corrected.
					_log(muon::LOG_ERROR) << "An exception '%s' occurred. "
						<< "Please correct the code and try again:\n\t>"
						<< _context->GetExceptionString()
						<< endl;
					return SCRIPT_FAILED_EXECUTE;
				}
			}
			//*/
			return SCRIPT_SUCCESS;
		}

		bool Script::eval(const muon::String& script)
		{
			return true;
		}

		bool Script::function(const muon::String& name)
		{
			return true;
		}
	}
}
