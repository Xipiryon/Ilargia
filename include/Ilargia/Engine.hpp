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

#ifndef _ILARGIA_ENGINE_H_INCLUDED
#define _ILARGIA_ENGINE_H_INCLUDED

//C/Cpp Standard
#include <ctime>
#include <vector>

//Ilargia Files that should be everywhere Engine.h is
#include <Muon/Core/Constant.hpp>
#include <Muon/Core/Typedef.hpp>
#include <Muon/Core/NonCopyable.hpp>
#include <Muon/System/KeyValue.hpp>
#include <Muon/System/Log.hpp>
#include <Muon/System/Time.hpp>
#include "Ilargia/Core/Define.hpp"
#include "Ilargia/System/Script.hpp"

//Base classes required by extended modules
#include "Ilargia/Component/EntityManager.hpp"
#include "Ilargia/Component/IComponentManager.hpp"
#include "Ilargia/Component/ComponentManagerFactory.hpp"

/*!
* @brief Main engine namespace
*/
namespace ilg
{
#ifdef __cplusplus
#	define extern_C_BEGIN extern "C" {
#	define extern_C_END }
#else
#	define extern_C_BEGIN
#	define extern_C_END
#endif

#define DNA_UNDEFINED_LIBRARY_NAME DNA_UNDEFINED_LIBRARY_NAME
#ifndef DNA_LIBRARY_NAME
#	define DNA_LIBRARY_NAME DNA_UNDEFINED_LIBRARY_NAME
#endif

	/*!
	* @brief Check Library filename
	* The library filename will be used to retrieve load/unload
	* functions. It must follow the same rules as any C/C++ function declaration.
	*/
#define DNA_LIBRARY_CHECK_FILENAME struct IlargiaCheckName { \
		IlargiaCheckName() { dna::String _s = DNA_STR(DNA_LIBRARY_NAME); if(_s == DNA_STR(DNA_UNDEFINED_LIBRARY_NAME)) { \
		DNA_ERROR("DNA_LIBRARY_NAME is not defined. It should be the library name: aborting, as it won't be possible to load it."); \
		exit(-1); } } \
	}; namespace dna { namespace priv { static IlargiaCheckName _check; } }

#define _ILARGIA_LIBRARY_LOAD_FUNC		DNA_TOKEN(DNA_LIBRARY_NAME, _load)
#define _ILARGIA_LIBRARY_UNLOAD_FUNC	DNA_TOKEN(DNA_LIBRARY_NAME, _unload)

	/*!
	* @brief
	*/
#define DNA_LIBRARY_INIT_BEGIN		extern_C_BEGIN int DNA_API _ILARGIA_LIBRARY_LOAD_FUNC (int argc, char** argv, char* _ilargia_error) {
	/*!
	* @brief
	*/
#define DNA_LIBRARY_INIT_END		} extern_C_END

	/*!
	* @brief
	*/
#define DNA_LIBRARY_TERM_BEGIN		extern_C_BEGIN void DNA_API _ILARGIA_LIBRARY_UNLOAD_FUNC () {
	/*!
	* @brief
	*/
#define DNA_LIBRARY_TERM_END		} extern_C_END

	/*!
	* @brief
	*/
#define DNA_LIBRARY_RETURN_SUCCESS	{return 0;};
	/*!
	*
	*/
#define DNA_LIBRARY_RETURN_FAILED	{::strcpy(_ilargia_error, error_str); return -1;};

	//class WorldModule;
	//class GraphicsModule;

	//! Engine functions
	class ILG_API Engine : public muon::NonCopyable
	{
	public:
		static void dispatchKeyCallback(void* windowHandle, int key, int scancode, int action, int modifier);

		static int main(int argc, char** argv);

		MUON_SINGLETON_GET(Engine);

		/*!
		* @brief Stop the engine
		*/
		static void stop();

		/*!
		* @brief Pause the engine
		* This force the delta time counter to 0.
		* @param pause Enable or disable the pause state
		*/
		static void pause(bool pause);

		/*!
		* @brief Toggle the pause/play state of the engine
		* This force the delta time counter to be set at 0 for the frame
		* the function is called.
		*/
		static void toggle();

		static muon::f32 getDeltaTime();
		static muon::f32 getProgramTime();
		static const char* getProgramPath();

		static bool isRunning();
		static bool isPaused();

	private:
		Engine();

		static bool init(int argc, char** argv);
		static void run();
		static void close();

		void _run();
		bool _loadConfig();
		bool _registerClasses();

		muon::system::Time _clock;
		muon::system::Log _log;
		bool _paused;
		bool _running;

		muon::f32 _deltaTime;
		muon::f32 _programTime;
		muon::String _programPath;
	};
}

#endif
