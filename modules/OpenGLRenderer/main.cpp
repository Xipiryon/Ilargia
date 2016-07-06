/*************************************************************************
* Ilargia Engine - http://github.com/Xleek/Ilargia
* C++ Modular Data Oriented Game Enginee
*------------------------------------------------------------------------
* Copyright (c) 2014 Louis Schnellbach
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

#include <Ilargia/Engine.hpp>
#include "OpenGLRenderer/OpenGLRenderer.hpp"
ilg::graphics::OpenGLRenderer* window = NULL;

ILARGIA_MODULE_CHECK_FILENAME()
ILARGIA_LIBRARY_INIT_BEGIN(argc, argv)
{
	window = MUON_NEW(ilg::graphics::OpenGLRenderer, "OpenGLRenderer", 5);
	ilg::manager::ManagerFactory::getInstance().registerComponentManager(window);
	ILARGIA_LIBRARY_RETURN_SUCCESS();
}
ILARGIA_LIBRARY_INIT_END()

ILARGIA_LIBRARY_TERM_BEGIN()
{
	MUON_DELETE(window);
}
ILARGIA_LIBRARY_TERM_END()
