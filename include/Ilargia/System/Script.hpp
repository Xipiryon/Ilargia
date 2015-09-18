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

#ifndef _ILARGIA_SCRIPT_H_INCLUDED
#define _ILARGIA_SCRIPT_H_INCLUDED

#include <unordered_map>

#include <Muon/Core/NonCopyable.hpp>
#include <Muon/System/Assert.hpp>
#include <Muon/System/Log.hpp>
#include "Ilargia/Core/Define.hpp"

class asIScriptEngine;
class asIScriptContext;
class asSMessageInfo;
namespace ilg
{
	/*!
	* @brief Indicates if a system::Script function was correctly executed
	*/
	enum ScriptState
	{
		SCRIPT_SUCCESS = 0,		//!< Everything was ok
		SCRIPT_FAILED_OPEN,		//!< File couldn't be opened
		SCRIPT_FAILED_BUILD,	//!< Script couldn't be build
		SCRIPT_FAILED_SECTION,	//!< Couldn't be added in Script Engine
		SCRIPT_FAILED_LOAD,		//!< Function couldn't be load
		SCRIPT_FAILED_PREPARE,	//!< Function couldn't be prepared correctly
		SCRIPT_FAILED_EXECUTE	//<! An error occured while executing a script
	};
	namespace system
	{
		/*!
		* @brief Helper function for Script registration
		* Provides a basic constructor function
		*/
		template<typename T> static void ScriptCstr(T* self) {new (self) T;}

		/*!
		* @brief Helper function for Script registration
		* Provides a basic destructor function
		*/
		template<typename T> static void ScriptDstr(T* self) { self->~T();}

		/*!
		* @brief Helper function for Script registration
		* Provides a basic copy constructor function
		*/
		template<typename T> static void ScriptCopy(T* self, const T& other) { new (self)T(other); }

		/*!
		* @brief Helper function for Script registration
		* Provides a basic assignment operator function
		*/
		template<typename T> static void ScriptAssign(T* self, const T& other) { self->operator=(other); }

		/*!
		* @brief Wrapper between the C++ engine and AngelScript
		*
		*/
		class ILG_API Script : public muon::NonCopyable
		{
		public:

			static void check(int);
			MUON_SINGLETON_GET(Script);

			asIScriptEngine* getScriptEngine() const;
			asIScriptContext* getScriptContext() const;

			/*!
			* @brief Register a C++ class into AngelScript
			* The class must implements a 'static void registerClass(asIScriptEngine* asEngine)' function.
			* @template T class to bind from C++ to AngelScript
			* @param str Ddisplaye the 'str' in a LOG_DEBUG level of output
			*/
			template<typename T>
			void registerScriptBinding(const char* str)
			{
				MUON_ASSERT(str != 0, "registerScriptBinding() as a NULL char array!");
				T::registerScriptBinding(getScriptEngine());
				_log(muon::LOG_DEBUG) << "Registered Script Binding: \"" << str << "\"!" << muon::endl;
			}

			/*!
			* @brief Load an AngelScript file
			* @param filename Path to the filename
			* @return SCRIPT_SUCCESS if the function as been correctly executed, corresponding error enum otherwise
			* @note The extension is required ("myfile" is different from "myfile.as") !
			* @note The program path will be added before the filename, unless specified as absolute path
			*/
			ScriptState load(const muon::String& filename);

			/*!
			* @brief Load an AngelScript file
			* @param filename Path to the filename
			* @param moduleName Give a name for the loaded module
			* @return SCRIPT_SUCCESS if the function as been correctly executed, corresponding error enum otherwise
			* @note The extension is required ("myfile" is different from "myfile.as") !
			* @note The program path will be added before the filename, unless specified as absolute path
			*/
			ScriptState load(const muon::String& filename, const muon::String& moduleName);

			/*!
			* @brief Compile the default module
			* @return SCRIPT_SUCCESS if the function as been correctly executed, corresponding error enum otherwise
			*/
			ScriptState compile();

			/*!
			* @brief Compile the default module
			* @param moduleName A specific module name
			* @return SCRIPT_SUCCESS if the function as been correctly executed, corresponding error enum otherwise
			*/
			ScriptState compile(const muon::String& moduleName);

			/*!
			* @brief Prepare the function
			* @param Script function signature (must be complete, like "int main(Object&in)"
			* @param Pointer to asISCriptContext pointer, allowing to add parameter to the function
			* @return SCRIPT_SUCCESS if the function as been correctly executed, corresponding error enum otherwise
			*/
			ScriptState prepare(const muon::String& funcName, asIScriptContext** ctx = NULL);

			/*!
			* @brief Prepare the function
			* @param Script function signature (must be complete, like "int main(Object&in)"
			* @param moduleName Name of the module in which the file containing the function has been loaded
			* @param Pointer to asISCriptContext pointer, allowing to add parameter to the function
			* @return SCRIPT_SUCCESS if the function as been correctly executed, corresponding error enum otherwise
			*/
			ScriptState prepare(const muon::String& funcName, const muon::String& moduleName, asIScriptContext** ctx = NULL);

			/*!
			* @brief Execute the previously loaded function
			* @return SCRIPT_SUCCESS if the function as been correctly executed, corresponding error enum otherwise
			*/
			ScriptState execute();

			bool eval(const muon::String& script);
			bool function(const muon::String& name);

			// Message callback used by AngelScript
			void errorCallback(const asSMessageInfo* msg);
		private:
			Script();
			virtual ~Script();

			void __registerClassBinding(void(*)(asIScriptEngine*), const char*);
			ScriptState _load(const muon::String& filename, const muon::String& moduleName);

			muon::system::Log _log;
			asIScriptEngine* _engine;
			asIScriptContext* _context;
			std::unordered_map<muon::String, bool>* _moduleCompiled;
		};
	}
}

#endif

