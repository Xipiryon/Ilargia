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

#ifndef _DNA_SHAREDLIBRARY_H_INCLUDED
#define _DNA_SHAREDLIBRARY_H_INCLUDED

#include <vector>

#include <Muon/Core/NonCopyable.hpp>
#include <Muon/System/Log.hpp>
#include "Ilargia/Core/Define.hpp"

namespace ilg
{
	class ComponentManagerFactory;
	struct SharedLibraryInfo
	{
		typedef int(*FuncLoad)(int, char**, char*);
		typedef void(*FuncUnload)();

		void* libInstance;
		muon::String name;
		muon::String funcLoadName;
		muon::String funcUnloadName;
		FuncLoad funcLoadPtr;
		FuncUnload funcUnloadPtr;
	};

	struct CManagerPair
	{
		IComponentManager *manager;
		void* library;
	};

	class SharedLibrary : public muon::NonCopyable
	{
	public:
		MUON_SINGLETON_GET(SharedLibrary);

		const std::vector<CManagerPair>& getManagers() const;
		const std::vector<SharedLibraryInfo>& getLibraries() const;

		bool loadLibrary(const muon::String& file, const muon::String& path);

		void forwardArg(int, char**);
		void setLinkStatic();
		void unloadLibraries();

	private:
		int _argc;
		char** _argv;

		friend class ComponentManagerFactory;
		friend class Engine;

		void _addModuleRef(IComponentManager*);
		bool _loadLibrary(SharedLibraryInfo&, const char*);
		bool _loadLibraryLoad(SharedLibraryInfo&, const char*);
		bool _loadLibraryUnload(SharedLibraryInfo&, const char*);
		void _closeLibrary(SharedLibraryInfo&);

		SharedLibrary();
		~SharedLibrary();

		bool _static;
		muon::system::Log _log;
		std::vector<SharedLibraryInfo> _libraries;
		std::vector<CManagerPair> _managers;
	};

}

#endif
