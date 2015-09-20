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

#include <sstream>
#include "Ilargia/Engine.hpp"

#if defined(MUON_PLATFORM_LINUX)
#   include <dlfcn.h>
#elif defined(MUON_PLATFORM_WINDOWS)
#	include <windows.h>
#elif defined(MUON_PLATFORM_APPLE)
#endif

#include "Ilargia/Component/IComponentManager.hpp"
#include "SharedLibrary.hpp"

namespace
{
	static void* currentLibRef = NULL;
}

namespace ilg
{
	SharedLibrary::SharedLibrary()
		: _argc(0)
		, _argv(NULL)
		, _static(false)
		, _log("SharedLibrary")
	{
	}

	SharedLibrary::~SharedLibrary()
	{
		for(auto it = _libraries.begin(); it != _libraries.end(); ++it)
		{
			_closeLibrary(*it);
		}
	}

	const std::vector<SharedLibraryInfo>& SharedLibrary::getLibraries() const
	{
		return _libraries;
	}

	const std::vector<CManagerPair>& SharedLibrary::getManagers() const
	{
		return _managers;
	}

	void SharedLibrary::forwardArg(int argc, char** argv)
	{
		_argc = argc;
		_argv = argv;
	}

	void SharedLibrary::setLinkStatic()
	{
		_static = true;
	}

	void SharedLibrary::_addModuleRef(IComponentManager* manager)
	{
		_managers.push_back({ manager, currentLibRef });
		_log(muon::LOG_DEBUG) << "Manager added: \"" << manager->getManagerName() << "\"" << muon::endl;
	}

	void SharedLibrary::unloadLibraries()
	{
		for(auto it = _libraries.rbegin(); it != _libraries.rend(); ++it)
		{
			if(it->funcUnloadPtr)
			{
				_log(muon::LOG_DEBUG) << "Unloading \"" << it->name << "\"" << muon::endl;
				(*(it->funcUnloadPtr))();
			}
			else
			{
				_log(muon::LOG_WARNING) << "No unload function for \"" << it->name << "\", skipping..." << muon::endl;
			}

			for (muon::i32 i = _managers.size() - 1; i != -1; --i)
			{
				if (_managers[i].library == it->libInstance)
				{
					IComponentManager* manager = _managers[i].manager;
					_log(muon::LOG_INFO) << "Deleting IComponentManager: \"" << manager->getManagerName() << "\"" << muon::endl;
					MUON_DELETE(manager);
					_managers.erase(_managers.begin() + i);
				}
			}
		}
	}

	bool SharedLibrary::loadLibrary(const muon::String& file, const muon::String& path)
	{
		muon::String libPath;
		//If not root, append the programPath
		if(path[0] != muon::PATH_SEPARATOR)
		{
			libPath = Engine::getProgramPath();
		}
		libPath += path;

		SharedLibraryInfo lib;
		lib.name = file;

		muon::String func = file.replace("-", "_");
		lib.funcLoadName = func + "_load";
		lib.funcUnloadName = func + "_unload";

		_log(muon::LOG_INFO) << "Loading \"" << libPath << "\"" << muon::endl;

		if (!_loadLibrary(lib, (_static ? NULL : libPath.cStr())))
		{
			currentLibRef = NULL;
			return false;
		}
		if (!_loadLibraryLoad(lib, lib.funcLoadName.cStr()))
		{
			currentLibRef = NULL;
			return false;
		}
		if (!_loadLibraryUnload(lib, lib.funcUnloadName.cStr()))
		{
			currentLibRef = NULL;
			return false;
		}

		_libraries.push_back(lib);
		return true;
	}

	bool SharedLibrary::_loadLibrary(SharedLibraryInfo& lib, const char* c_libPath)
	{
#if defined(MUON_PLATFORM_WINDOWS)
		HINSTANCE lib_handle;
		lib_handle = LoadLibrary(c_libPath);
		LPTSTR lpErrorText = NULL;
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER
			, 0, GetLastError(), 0, lpErrorText, MAX_PATH, 0);
		const char* error = (lpErrorText == NULL ? "File doesn't exist" : lpErrorText);
		LocalFree(lpErrorText);
#elif defined(MUON_PLATFORM_LINUX) || defined(MUON_PLATFORM_APPLE)
		void* lib_handle = dlopen(c_libPath, RTLD_LAZY);
		char* error = dlerror();
#endif
		if (!lib_handle)
		{
			_log(muon::LOG_ERROR) << "Couldn't load \"" << c_libPath << "\": " << error << muon::endl;
			return false;
		}

		lib.libInstance = (void*)lib_handle;
		currentLibRef = lib.libInstance;
		return true;
	}

	bool SharedLibrary::_loadLibraryLoad(SharedLibraryInfo& lib, const char* c_funcLoad)
	{
		if (c_funcLoad)
		{
			char error[2048];
			error[0] = 0;
#if defined(MUON_PLATFORM_WINDOWS)
			HINSTANCE lib_handle = (HINSTANCE)lib.libInstance;
			lib.funcLoadPtr = (SharedLibraryInfo::FuncLoad)GetProcAddress(lib_handle, c_funcLoad);
			if (!lib.funcLoadPtr)
			{
				strcpy(error, "Couldn't find any function with that name.");
#elif defined(MUON_PLATFORM_LINUX) || defined(MUON_PLATFORM_APPLE)
			void* lib_handle = lib.libInstance;
			lib.funcLoadPtr = (SharedLibraryInfo::FuncLoad)dlsym(lib_handle, c_funcLoad);
			const char* cerror = dlerror();
			if(cerror)
			{
				strcpy(error, cerror);
			}
			if (error[0] != 0)
			{
#endif
				_log(muon::LOG_ERROR) << "Couldn't find any 'load' function \"" << c_funcLoad << "\": " << error << muon::endl;
				_closeLibrary(lib);
				return false;
			}

			error[0] = 0;
			// error handle
			if(((*(lib.funcLoadPtr))(_argc, _argv, error)) != 0)
			{
				_log(muon::LOG_ERROR) << "Library \"" << lib.name << "\" exited with error: \"" << error << "\"" << muon::endl;
				_closeLibrary(lib);
				return false;
			}
		}
		else
		{
			lib.funcLoadPtr = NULL;
			_log(muon::LOG_WARNING) << "'load' function not specified, skipping..." << muon::endl;
		}
		return true;
	}

	bool SharedLibrary::_loadLibraryUnload(SharedLibraryInfo& lib, const char* c_funcUnload)
	{
		if (c_funcUnload)
		{
			muon::String error;
#if defined(MUON_PLATFORM_WINDOWS)
			HINSTANCE lib_handle = (HINSTANCE)lib.libInstance;
			lib.funcUnloadPtr = (SharedLibraryInfo::FuncUnload)GetProcAddress(lib_handle, c_funcUnload);
			if (!lib.funcLoadPtr)
			{
				error = "Couldn't find any function with that name.";
#elif defined(MUON_PLATFORM_LINUX) || defined(MUON_PLATFORM_APPLE)
			void* lib_handle = lib.libInstance;
			lib.funcUnloadPtr = (SharedLibraryInfo::FuncUnload)dlsym(lib_handle, c_funcUnload);
			error = dlerror();
			if(!error.empty())
			{
#endif
				_log(muon::LOG_ERROR) << "Couldn't find any 'unload' function \"" << c_funcUnload << "\": " << error << muon::endl;
				_closeLibrary(lib);
				return false;
			}
		}
		else
		{
			lib.funcUnloadPtr = NULL;
			_log(muon::LOG_WARNING) << "'unload' function not specified, skipping..." << muon::endl;
		}
		return true;
	}

	void SharedLibrary::_closeLibrary(SharedLibraryInfo& lib)
	{
		if(!_static)
		{
#if defined(MUON_PLATFORM_WINDOWS)
			FreeLibrary((HINSTANCE)lib.libInstance);
#elif defined(MUON_PLATFORM_LINUX) || defined(MUON_PLATFORM_APPLE)
			dlclose(lib.libInstance);
#endif
		}
	}
}
