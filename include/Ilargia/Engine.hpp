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

#ifndef _ILARGIA_ENGINE_H_INCLUDED
#define _ILARGIA_ENGINE_H_INCLUDED

//C/Cpp Standard
#include <ctime>
#include <vector>

//Ilargia Files that should be everywhere Engine.h is
#include <Muon/Core/Constant.hpp>
#include <Muon/Core/Typedef.hpp>
#include <Muon/Helper/NonCopyable.hpp>
#include <Muon/System/KeyValue.hpp>
#include <Muon/System/Log.hpp>
#include <Muon/System/Time.hpp>
#include "Ilargia/Core/Define.hpp"

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
#	define EXTERN_C_BEGIN extern "C" {
#	define EXTERN_C_END }
#else
#	define EXTERN_C_BEGIN
#	define EXTERN_C_END
#endif

	/*!
	* @brief Check Library filename
	* The library filename will be used to retrieve load/unload
	* functions. It must follow the same rules as any C/C++ function declaration.
	*/
#define ILARGIA_MODULE_CHECK_FILENAME struct IlargiaCheckName { \
		IlargiaCheckName() { muon::String _s = MUON_STR(ILARGIA_MODULE_NAME); if(_s.empty()) { \
		MUON_ERROR("ILARGIA_MODULE_NAME is not defined! Please check your module has been correctly compiled."); \
		exit(-1); } } \
	}; namespace ilg { namespace priv { static IlargiaCheckName _check; } }

#define _ILARGIA_MODULE_LOAD_FUNC		MUON_GLUE(ILARGIA_MODULE_NAME, _load)
#define _ILARGIA_MODULE_UNLOAD_FUNC		MUON_GLUE(ILARGIA_MODULE_NAME, _unload)

	/*!
	* @brief
	*/
#define ILARGIA_MODULE_INIT_BEGIN		EXTERN_C_BEGIN int ILARGIA_API _ILARGIA_MODULE_LOAD_FUNC (int argc, char** argv, char* _ilargia_error) {
	/*!
	* @brief
	*/
#define ILARGIA_MODULE_INIT_END		} EXTERN_C_END

	/*!
	* @brief
	*/
#define ILARGIA_MODULE_TERM_BEGIN		EXTERN_C_BEGIN void ILARGIA_API _ILARGIA_MODULE_UNLOAD_FUNC () {
	/*!
	* @brief
	*/
#define ILARGIA_MODULE_TERM_END		} EXTERN_C_END

	/*!
	* @brief
	*/
#define ILARGIA_MODULE_RETURN_SUCCESS	{return 0;};
	/*!
	*
	*/
#define ILARGIA_MODULE_RETURN_FAILED	{::strcpy(_ilargia_error, error_str); return -1;};

	//class WorldModule;
	//class GraphicsModule;

	//! Engine functions
	class ILG_API Engine : public muon::helper::Singleton<Engine>
	{
		friend class muon::helper::Singleton<Engine>;
	public:
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
