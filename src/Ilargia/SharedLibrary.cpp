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

#include <sstream>
#include "Ilargia/Engine.hpp"

#if defined(MUON_PLATFORM_LINUX)
#   include <dlfcn.h>
#elif defined(MUON_PLATFORM_WINDOWS)
#	include <windows.h>
#elif defined(MUON_PLATFORM_APPLE)
#endif

#include "Ilargia/Manager/IBaseManager.hpp"
#include "SharedLibrary.hpp"

namespace
{
	static void* currentLibRef = NULL;
}

namespace ilg
{
	SharedLibrary::SharedLibrary()
		: m_argc(0)
		, m_argv(NULL)
		, m_log("SharedLibrary")
	{
	}

	SharedLibrary::~SharedLibrary()
	{
		for (auto it = m_libraries.begin(); it != m_libraries.end(); ++it)
		{
			_closeLibrary(*it);
		}
	}

	const std::vector<SharedLibraryInfo>& SharedLibrary::getLibraries() const
	{
		return m_libraries;
	}

	const std::vector<CManagerPair>& SharedLibrary::getManagers() const
	{
		return m_managers;
	}

	void SharedLibrary::forwardArg(int argc, char** argv)
	{
		m_argc = argc;
		m_argv = argv;
	}

	void SharedLibrary::_addModuleRef(manager::IBaseManager* manager)
	{
		m_managers.push_back({ manager, currentLibRef });
		m_log(m::LOG_DEBUG) << "Manager added: \"" << manager->getManagerName() << "\"" << m::endl;
	}

	void SharedLibrary::unloadLibraries()
	{
		for (auto it = m_libraries.rbegin(); it != m_libraries.rend(); ++it)
		{
			if (it->funcUnloadPtr)
			{
				m_log(m::LOG_DEBUG) << "Unloading \"" << it->name << "\"" << m::endl;
				(*(it->funcUnloadPtr))();
			}
			else
			{
				m_log(m::LOG_WARNING) << "No unload function for \"" << it->name << "\", skipping..." << m::endl;
			}
		}
	}

	bool SharedLibrary::loadLibrary(const m::String& name, const m::String& filepath)
	{
		SharedLibraryInfo lib;
		lib.name = name;

		m::String func = name.replace("-", "_").replace(" ", "_");
		lib.funcLoadName = func + "_load";
		lib.funcUnloadName = func + "_unload";

		m_log(m::LOG_INFO) << "Loading \"" << name << "\" from \"" << filepath << "\"" << m::endl;

		if (!_loadLibrary(lib, filepath.cStr()))
		{
			currentLibRef = NULL;
			return false;
		}
		if (!_loadLibrary_LoadFunction(lib, lib.funcLoadName.cStr()))
		{
			currentLibRef = NULL;
			return false;
		}
		if (!_loadLibrary_UnloadFunction(lib, lib.funcUnloadName.cStr()))
		{
			currentLibRef = NULL;
			return false;
		}

		m_libraries.push_back(lib);
		return true;
	}

	bool SharedLibrary::_loadLibrary(SharedLibraryInfo& lib, const char* c_libPath)
	{
#if !defined(ILARGIA_LINK_STATIC)
#	if defined(MUON_PLATFORM_WINDOWS)
		HMODULE lib_handle = LoadLibrary(c_libPath);
		LPTSTR lpErrorText = NULL;
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER
					  , 0, GetLastError(), 0, lpErrorText, MAX_PATH, 0);
		const char* error = (lib_handle == NULL ? "File not found!" : (lpErrorText == NULL ? "Unknown error" : lpErrorText));
		LocalFree(lpErrorText);
#	elif defined(MUON_PLATFORM_LINUX) || defined(MUON_PLATFORM_APPLE)
		void* lib_handle = dlopen(c_libPath, RTLD_LAZY);
		char* error = dlerror();
#	endif
		if (!lib_handle)
		{
			m_log(m::LOG_ERROR) << "Couldn't load \"" << c_libPath << "\": " << error << m::endl;
			return false;
		}
#else
		void* lib_handle = NULL;
#endif
		lib.libInstance = (void*)lib_handle;
		return true;
	}

	bool SharedLibrary::_loadLibrary_LoadFunction(SharedLibraryInfo& lib, const char* c_funcLoad)
	{
		m::String error;
		bool errorOccured = false;
		if (c_funcLoad)
		{
#if !defined(ILARGIA_LINK_STATIC)
#	if defined(MUON_PLATFORM_WINDOWS)
			HINSTANCE lib_handle = (HINSTANCE)lib.libInstance;
			lib.funcLoadPtr = (SharedLibraryInfo::FuncLoad)GetProcAddress(lib_handle, c_funcLoad);
			if (!lib.funcLoadPtr)
			{
				error = "Couldn't find any function with that name.";
				errorOccured = true;
			}
#	elif defined(MUON_PLATFORM_LINUX) || defined(MUON_PLATFORM_APPLE)
			void* lib_handle = lib.libInstance;
			lib.funcLoadPtr = (SharedLibraryInfo::FuncLoad)dlsym(lib_handle, c_funcLoad);
			error = dlerror();
			if (!error.empty())
			{
				errorOccured = true;
			}
#	endif
#else
#endif
			if (errorOccured)
			{
				m_log(m::LOG_ERROR) << "Couldn't find any 'load' function \"" << c_funcLoad << "\": " << error << m::endl;
				_closeLibrary(lib);
				return false;
			}

			if (((*(lib.funcLoadPtr))(m_argc, m_argv)) != 0)
			{
				m_log(m::LOG_ERROR) << "Library \"" << lib.name << "\" couldn't be loaded!" << m::endl;
				_closeLibrary(lib);
				return false;
			}
		}
		else
		{
			lib.funcLoadPtr = NULL;
			m_log(m::LOG_WARNING) << "'load' function not specified, skipping..." << m::endl;
		}
		return true;
	}

	bool SharedLibrary::_loadLibrary_UnloadFunction(SharedLibraryInfo& lib, const char* c_funcUnload)
	{
		m::String error;
		bool errorOccured = false;
		if (c_funcUnload)
		{
#if !defined(ILARGIA_LINK_STATIC)
#	if defined(MUON_PLATFORM_WINDOWS)
			HINSTANCE lib_handle = (HINSTANCE)lib.libInstance;
			lib.funcUnloadPtr = (SharedLibraryInfo::FuncUnload)GetProcAddress(lib_handle, c_funcUnload);
			if (!lib.funcLoadPtr)
			{
				error = "Couldn't find any function with that name.";
				errorOccured = true;
			}
#	elif defined(MUON_PLATFORM_LINUX) || defined(MUON_PLATFORM_APPLE)
			void* lib_handle = lib.libInstance;
			lib.funcUnloadPtr = (SharedLibraryInfo::FuncUnload)dlsym(lib_handle, c_funcUnload);
			error = dlerror();
			if (!error.empty())
			{
				errorOccured = true;
			}
#	endif
#else
#endif
			if (errorOccured)
			{
				m_log(m::LOG_ERROR) << "Couldn't find any 'unload' function \"" << c_funcUnload << "\": " << error << m::endl;
				_closeLibrary(lib);
				return false;
			}
		}
		else
		{
			lib.funcUnloadPtr = NULL;
			m_log(m::LOG_WARNING) << "'unload' function not specified, skipping..." << m::endl;
		}
		return true;
	}

	void SharedLibrary::_closeLibrary(SharedLibraryInfo& lib)
	{
#if !defined(ILARGIA_LINK_STATIC)
#	if defined(MUON_PLATFORM_WINDOWS)
		FreeLibrary((HINSTANCE)lib.libInstance);
#	elif defined(MUON_PLATFORM_LINUX) || defined(MUON_PLATFORM_APPLE)
		dlclose(lib.libInstance);
#	endif
#endif
	}
}
