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

#ifndef INCLUDE_ILARGIA_OPENGLRENDERER_HPP
#define INCLUDE_ILARGIA_OPENGLRENDERER_HPP

#include <Ilargia/Engine.hpp>
#include <Ilargia/Manager/ISimpleManager.hpp>

struct GLFWwindow;
namespace ilg
{
	namespace graphics
	{
		class ILARGIA_API OpenGLRenderer : public ilg::manager::ISimpleManager
		{
		public:
			OpenGLRenderer(const m::String& name, m::i32 updateOrder);
			virtual ~OpenGLRenderer();

			virtual void onInit();
			virtual void onUpdate(m::f32);
			virtual void onTerm();

			virtual void onKeyCallback(void* windowHandle, int key, int scancode, int action, int modifier);

			void setName(const m::String& name);
			void setFullscreen(bool fullscreen);
			void setDeferred(bool deferred);
			void setWidth(m::u32 width);
			void setHeight(m::u32 height);

			const m::String& getName() const;
			bool getFullscreen() const;
			bool getDeferred() const;
			m::u32 getWidth() const;
			m::u32 getHeight() const;

		private:
			void _run();
			bool m_initialized;
			GLFWwindow* m_window;

			m::String m_name;
			bool m_fullscreen;
			m::u32 m_width;
			m::u32 m_height;
		};
	}
}

#endif
