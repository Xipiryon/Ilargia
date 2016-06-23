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

#include <iostream>
#include <thread>
#include <Muon/Core/Define.hpp>

#include <gl3w.h>
#include <Ilargia/Graphics/OpenGL/VertexBufferObject.hpp>

#include "OpenGLRenderer/GLFW/glfw3.h"
#include "OpenGLRenderer/OpenGLRenderer.hpp"

void errorCallback(int error, const char* desc)
{
	m::system::Log("[GLFW]", m::LOG_ERROR) << "Error{ " << error << " } " << desc << m::endl;
}

namespace ilg
{
	namespace graphics
	{
		OpenGLRenderer::OpenGLRenderer(const m::String& name, m::i32 updateOrder)
			: ISimpleManager(name, updateOrder)
			, m_initialized(false)
			, m_window(NULL)
			, m_name("GLFW Window")
			, m_fullscreen(false)
			, m_width(800)
			, m_height(600)
		{
			//Set default log to LOG_INFO
			getLog(m::LOG_INFO);
		}

		OpenGLRenderer::~OpenGLRenderer()
		{
		}

		void OpenGLRenderer::onInit()
		{
			if (!glfwInit())
			{
				getLog(m::LOG_ERROR) << "Failed to initialize GLFW " << m::endl;
				return;
			}

			m_window = glfwCreateWindow(m_width, m_height, m_name.cStr()
										, (m_fullscreen ? glfwGetPrimaryMonitor() : NULL)
										, NULL);
			if (!m_window)
			{
				glfwTerminate();
				return;
			}
			glfwMakeContextCurrent(m_window);
			glfwSetKeyCallback(m_window, (GLFWkeyfun)Engine::dispatchKeyCallback);
			glfwSetErrorCallback(errorCallback);

			if (gl3wInit())
			{
				getLog(m::LOG_ERROR) << "Failed to initialize GL3W " << m::endl;
				return;
			}
			//Setup OpenGL value
			glClearDepth(1.f);
			glClearColor(0.5f, 0.7f, 1.f, 1.f);

			glEnable(GL_DEPTH_TEST);
			glEnable(GL_CULL_FACE);
			glFrontFace(GL_CCW);
			glCullFace(GL_BACK);

			//Graphic Cards info
			m::String glslVersion = (char*)glGetString(GL_SHADING_LANGUAGE_VERSION);

			getLog(m::LOG_INFO);
			getLog() << "----------" << m::endl;
			getLog() << "GL_VENDOR: " << (const char*)glGetString(GL_VENDOR) << m::endl;
			getLog() << "GL_RENDERER: " << (const char*)glGetString(GL_RENDERER) << m::endl;
			getLog() << "GL_VERSION: " << (const char*)glGetString(GL_VERSION) << m::endl;
			getLog() << "GL_SHADING: " << glslVersion << m::endl;
			getLog() << "----------" << m::endl;
			m_initialized = true;
		}

		void OpenGLRenderer::onUpdate(m::f32 dt)
		{
			if (!m_initialized)
			{
				return;
			}

			// Poll Windows Even
			glfwPollEvents();

			// Compute rendering
			glClear(GL_COLOR_BUFFER_BIT);

			// Render Frame
			glfwSwapBuffers(m_window);

			if (glfwWindowShouldClose(m_window))
			{
			}
		}

		void OpenGLRenderer::onTerm()
		{
			glfwDestroyWindow(m_window);
			glfwTerminate();
		}

		void OpenGLRenderer::onKeyCallback(void* windowHandle, int key, int scancode, int action, int modifier)
		{
			GLFWwindow* glfw = (GLFWwindow*)windowHandle;
			getLog(m::LOG_DEBUG) << "Received Key: " << m::endl
				<< "\t>Key: " << key << m::endl
				<< "\t>ScanCode: " << scancode << m::endl
				<< "\t>Action: " << action << m::endl
				<< "\t>Modifier: " << modifier << m::endl;

			if (key == GLFW_KEY_ESCAPE)
			{
				Engine::stop();
			}
		}

		void OpenGLRenderer::setName(const m::String& name)
		{
			m_name = name;
		}

		void OpenGLRenderer::setFullscreen(bool fullscreen)
		{
			m_fullscreen = fullscreen;
		}

		void OpenGLRenderer::setWidth(m::u32 width)
		{
			m_width = width;
		}

		void OpenGLRenderer::setHeight(m::u32 height)
		{
			m_height = height;
		}

		const m::String& OpenGLRenderer::getName() const
		{
			return m_name;
		}

		bool OpenGLRenderer::getFullscreen() const
		{
			return m_fullscreen;
		}

		m::u32 OpenGLRenderer::getWidth() const
		{
			return m_width;
		}

		m::u32 OpenGLRenderer::getHeight() const
		{
			return m_height;
		}
	}
}
