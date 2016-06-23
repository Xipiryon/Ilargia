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

#ifndef INCLUDE_ILARGIA_SCRIPTDRIVER_HPP
#define INCLUDE_ILARGIA_SCRIPTDRIVER_HPP

#include <unordered_map>
#include <Muon/Helper/NonCopyable.hpp>
#include <Muon/System/Log.hpp>
#include "Ilargia/System/IScriptContext.hpp"
#include "Ilargia/System/IScriptEngine.hpp"

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
		* @brief Wrapper between the C++ engine and the script language
		*
		*
		*/
		class ILARGIA_API ScriptDriver : public m::helper::NonCopyable
		{
		public:
			MUON_SINGLETON_GET(ScriptDriver);

			static void check(int);

			IScriptEngine* getScriptEngine() const;
			IScriptContext* getScriptContext() const;

			/*!
			* @brief Load an AngelScript file
			* @param filename Path to the filename
			* @return SCRIPT_SUCCESS if the function as been correctly executed, corresponding error enum otherwise
			* @note The extension is required ("myfile" is different from "myfile.as") !
			* @note The program path will be added before the filename, unless specified as absolute path
			*/
			ScriptState load(const m::String& filename);

			/*!
			* @brief Load an AngelScript file
			* @param filename Path to the filename
			* @param moduleName Give a name for the loaded module
			* @return SCRIPT_SUCCESS if the function as been correctly executed, corresponding error enum otherwise
			* @note The extension is required ("myfile" is different from "myfile.as") !
			* @note The program path will be added before the filename, unless specified as absolute path
			*/
			ScriptState load(const m::String& filename, const m::String& moduleName);

			/*!
			* @brief Compile the default module
			* @return SCRIPT_SUCCESS if the function as been correctly executed, corresponding error enum otherwise
			*/
			virtual ScriptState compile();

			/*!
			* @brief Compile the default module
			* @param moduleName A specific module name
			* @return SCRIPT_SUCCESS if the function as been correctly executed, corresponding error enum otherwise
			*/
			virtual ScriptState compile(const m::String& moduleName);

			/*!
			* @brief Prepare the function
			* @param Script function signature (must be complete, like "int main(Object&in)"
			* @param Pointer to asISCriptContext pointer, allowing to add parameter to the function
			* @return SCRIPT_SUCCESS if the function as been correctly executed, corresponding error enum otherwise
			*/
			virtual ScriptState prepare(const m::String& funcName, IScriptContext** ctx = NULL);

			/*!
			* @brief Prepare the function
			* @param Script function signature (must be complete, like "int main(Object&in)"
			* @param moduleName Name of the module in which the file containing the function has been loaded
			* @param Pointer to asISCriptContext pointer, allowing to add parameter to the function
			* @return SCRIPT_SUCCESS if the function as been correctly executed, corresponding error enum otherwise
			*/
			virtual ScriptState prepare(const m::String& funcName, const m::String& moduleName, IScriptContext** ctx = NULL);

			/*!
			* @brief Execute the previously loaded function
			* @return SCRIPT_SUCCESS if the function as been correctly executed, corresponding error enum otherwise
			*/
			virtual ScriptState execute();

			virtual bool eval(const m::String& script);
			virtual bool function(const m::String& name);

		private:
			ScriptDriver();
			virtual ~ScriptDriver();

			void _errorCallback(const m::String& msg);
			ScriptState _load(const m::String& filename, const m::String& moduleName);

			m::system::Log m_log;
			IScriptEngine* m_engine;
			IScriptContext* m_context;
			std::unordered_map<m::String, bool>* m_moduleCompiled;
		};
	}
}

#endif
