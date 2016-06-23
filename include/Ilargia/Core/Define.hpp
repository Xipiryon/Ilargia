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
*	claim that you wrote the original software. If you use this software
*	in a product, an acknowledgment in the product documentation would
*	be appreciated but is not required.
*
* 2. Altered source versions must be plainly marked as such, and must not
*	be misrepresented as being the original software.
*
* 3. This notice may not be removed or altered from any source
*	distribution.
*
*************************************************************************/

#ifndef INCLUDE_ILARGIA_DEFINE_HPP
#define INCLUDE_ILARGIA_DEFINE_HPP

#include <Muon/Core/Define.hpp>
#include <Muon/Core/Typedef.hpp>

//		--------------------------
//				EXPORT
//		--------------------------
#if defined(ILARGIA_STATIC)
#	define ILARGIA_API
#else
//Windows
#	ifdef _MSC_VER
#		if ILARGIA_EXPORTS
#			define ILARGIA_API __declspec(dllexport)
#		else
#			define ILARGIA_API __declspec(dllimport)
#		endif
#	else
//Unix
#		if ILARGIA_EXPORTS
#			define ILARGIA_API __attribute__ ((visibility("default")))
#		else
#			define ILARGIA_API
#		endif
#	endif
#endif

#endif //INCLUDE_ILARGIA_DEFINE_HPP
