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

#ifndef INCLUDE_ILARGIA_ENGINE_HPP
#define INCLUDE_ILARGIA_ENGINE_HPP

//C/Cpp Standard
#include <ctime>
#include <vector>

//Ilargia Files that should be everywhere Engine.h is
#include <Muon/Core/Constant.hpp>
#include <Muon/Core/Typedef.hpp>
#include <Muon/Helper/NonCopyable.hpp>
#include <Muon/System/Log.hpp>
#include <Muon/System/Time.hpp>
#include "Ilargia/Core/Define.hpp"

//Base classes required by extended modules
#include "Ilargia/Component/Entity.hpp"
#include "Ilargia/Manager/IComponentManager.hpp"
#include "Ilargia/Manager/ISimpleManager.hpp"
#include "Ilargia/Manager/ManagerFactory.hpp"

/*!
* @brief Main engine namespace
*/
namespace ilg
{
#ifdef __cplusplus
#	define EXTERN_C_BEGIN extern "C" {
#	define EXTERN_C_END }
#else
#	define EXTERN_C_BEGIN
#	define EXTERN_C_END
#endif

#ifdef _DOXYGEN
	/*!
	* @brief Check Library filename
	* The library filename will be used to retrieve load/unload
	* functions. It must follow the same rules as any C/C++ function declaration.
	*/
#define ILARGIA_MODULE_CHECK_FILENAME
#else
#	ifndef ILARGIA_MODULE_NAME
#		define ILARGIA_MODULE_CHECK_FILENAME() static_assert(false, "ILARGIA_MODULE_NAME is not defined!");
#	else
#		define ILARGIA_MODULE_CHECK_FILENAME()
#	endif
#endif

#define _ILARGIA_LIBRARY_LOAD_FUNC		MUON_GLUE(ILARGIA_MODULE_NAME, _load)
#define _ILARGIA_LIBRARY_UNLOAD_FUNC		MUON_GLUE(ILARGIA_MODULE_NAME, _unload)
#if defined(ILARGIA_STATIC)
	void ILARGIA_API registerStaticLibraryLoadFunction(const m::String& name, void* fptr);
	void ILARGIA_API registerStaticLibraryUnloadFunction(const m::String& name, void* fptr);
#	define _ILARGIA_LIBRARY_REGISTER_STATIC_LOAD
#	define _ILARGIA_LIBRARY_REGISTER_STATIC_UNLOAD
#else
#	define _ILARGIA_LIBRARY_REGISTER_STATIC_LOAD
#	define _ILARGIA_LIBRARY_REGISTER_STATIC_UNLOAD
#endif

	/*!
	* @brief
	*/
#define ILARGIA_LIBRARY_INIT_BEGIN(argc, argv) EXTERN_C_BEGIN int ILARGIA_API _ILARGIA_LIBRARY_LOAD_FUNC (int argc, char** argv) {
	/*!
	* @brief
	*/
#define ILARGIA_LIBRARY_INIT_END()	} EXTERN_C_END

	/*!
	* @brief
	*/
#define ILARGIA_LIBRARY_TERM_BEGIN()		EXTERN_C_BEGIN void ILARGIA_API _ILARGIA_LIBRARY_UNLOAD_FUNC () {
	/*!
	* @brief
	*/
#define ILARGIA_LIBRARY_TERM_END()	} EXTERN_C_END

	/*!
	* @brief
	*/
#define ILARGIA_LIBRARY_RETURN_SUCCESS()	{return 0;};
	/*!
	*
	*/
#define ILARGIA_LIBRARY_RETURN_FAILED(error_str)		{return -1;};

	//! Engine functions
	class ILARGIA_API Engine : public m::helper::NonCopyable
	{
	public:
		MUON_SINGLETON_GET(Engine);

		static void dispatchKeyCallback(void* windowHandle, int key, int scancode, int action, int modifier);
		static int main(int argc, char** argv);

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

		static m::f32 getDeltaTime();
		static m::f32 getProgramTime();
		static const m::String& getProgramPath();

		static bool isRunning();
		static bool isPaused();

	private:
		Engine();

		static bool init(int argc, char** argv);
		static void run();
		static void close();

		void _run();
		bool _loadConfig();
		void _registerCoreClass();
		void _registerCoreComponentManager();

		m::system::Time m_clock;
		m::system::Log m_log;
		bool m_paused;
		bool m_running;

		m::f32 m_deltaTime;
		m::f32 m_programTime;
		m::String m_programPath;
	};
}

#endif
