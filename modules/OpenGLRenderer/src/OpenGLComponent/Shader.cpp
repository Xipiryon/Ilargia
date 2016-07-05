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

#include <fstream>
#include <Muon/System/Log.hpp>

#include "gl3w/gl3w.h"
#include "OpenGLComponent/Shader.hpp"

namespace ilg
{
	namespace graphics
	{
		Shader::Shader(const m::String& vertexShader, const m::String& fragmentShader) :
			m_initialized(false)
		{
			typedef std::unordered_map<m::String, GLuint> ShaderMapStruct;
			m_uniformMap = MUON_NEW(ShaderMapStruct);
			m_attribMap = MUON_NEW(ShaderMapStruct);

			m_vertexShader.filename = vertexShader;
			m_fragmentShader.filename = fragmentShader;
		}

		Shader::~Shader()
		{
			MUON_DELETE(m_uniformMap);
			MUON_DELETE(m_attribMap);
		}

		void Shader::unload()
		{
			glDetachShader(m_programID, m_vertexShader.id);
			glDetachShader(m_programID, m_fragmentShader.id);
			glDeleteShader(m_vertexShader.id);
			glDeleteShader(m_fragmentShader.id);
			glDeleteShader(m_programID);
			m_initialized = false;
		}

		bool Shader::initialize()
		{
			if (m_initialized)
			{
				return true;
			}

			m_programID = glCreateProgram();
			m_vertexShader.id = glCreateShader(GL_VERTEX_SHADER);
			m_fragmentShader.id = glCreateShader(GL_FRAGMENT_SHADER);

			m_vertexShader.source = readFile(m_vertexShader.filename);
			m_fragmentShader.source = readFile(m_fragmentShader.filename);

			if (m_vertexShader.source.empty() || m_fragmentShader.source.empty())
			{
				return false;
			}

			const GLchar* tmp = static_cast<const GLchar*>(m_vertexShader.source.cStr());
			glShaderSource(m_vertexShader.id, 1, (const GLchar**)&tmp, NULL);

			tmp = static_cast<const GLchar*>(m_fragmentShader.source.cStr());
			glShaderSource(m_fragmentShader.id, 1, (const GLchar**)&tmp, NULL);

			if (!compileShader(m_vertexShader) || !compileShader(m_fragmentShader))
			{
				return false;
			}

			glAttachShader(m_programID, m_vertexShader.id);
			glAttachShader(m_programID, m_fragmentShader.id);

			glLinkProgram(m_programID);
			m_initialized = true;
			return true;
		}

		void Shader::linkProgram()
		{
			glLinkProgram(m_programID);
		}

		GLuint Shader::getUniformLocation(const m::String& name)
		{
			auto it = m_uniformMap->find(name);
			if (it == m_uniformMap->end())
			{
				GLuint location = glGetUniformLocation(m_programID, name.cStr());
				m_uniformMap->insert(std::make_pair(name, location));
				return location;
			}

			return it->second;
		}

		GLuint Shader::getAttribLocation(const m::String& name)
		{
			auto it = m_attribMap->find(name);
			if (it == m_attribMap->end())
			{
				GLuint location = glGetAttribLocation(m_programID, name.cStr());
				m_attribMap->insert(std::make_pair(name, location));
				return location;
			}

			return it->second;
		}

		void Shader::sendUniform(const m::String& name, const int id)
		{
			GLuint location = getUniformLocation(name);
			glUniform1i(location, id);
		}

		void Shader::sendUniform4x4(const m::String& name, const m::f32* matrix, bool transpose)
		{
			GLuint location = getUniformLocation(name);
			glUniformMatrix4fv(location, 1, transpose, matrix);
		}

		void Shader::sendUniform3x3(const m::String& name, const m::f32* matrix, bool transpose)
		{
			GLuint location = getUniformLocation(name);
			glUniformMatrix3fv(location, 1, transpose, matrix);
		}

		void Shader::sendUniform(const m::String& name, const m::f32 red, const m::f32 green,
								 const m::f32 blue, const m::f32 alpha)
		{
			GLuint location = getUniformLocation(name);
			glUniform4f(location, red, green, blue, alpha);
		}

		void Shader::sendUniform(const m::String& name, const m::f32 x, const m::f32 y, const m::f32 z)
		{
			GLuint location = getUniformLocation(name);
			glUniform3f(location, x, y, z);
		}

		void Shader::sendUniform(const m::String& name, const m::f32 x, const m::f32 y)
		{
			GLuint location = getUniformLocation(name);
			glUniform2f(location, x, y);
		}

		void Shader::sendUniform(const m::String& name, const m::f32 scalar)
		{
			GLuint location = getUniformLocation(name);
			glUniform1f(location, scalar);
		}

		void Shader::bindAttrib(m::u32 index, const m::String& attribName)
		{
			glBindAttribLocation(m_programID, index, attribName.cStr());
		}

		void Shader::bindShader()
		{
			glUseProgram(m_programID);
		}

		m::String Shader::readFile(const m::String& filename)
		{
			std::ifstream fileIn(filename.cStr());

			if (!fileIn)
			{
				m::system::Log err("Shader", m::LOG_ERROR);
				err << "Could not load shader: \"" << filename << "\"" << m::endl;
				return m::String();
			}

			m::String stringBuffer;
			fileIn.seekg(0, fileIn.end);
			m::u64 length = (m::u64)fileIn.tellg();
			fileIn.seekg(0, fileIn.beg);

			char* buffer = (char*)malloc((m::u32)length + 1);
			fileIn.read(buffer, length);
			buffer[fileIn.gcount()] = 0;
			stringBuffer += buffer;
			free(buffer);

			return stringBuffer;
		}

		bool Shader::compileShader(const GLSLShader& shader)
		{
			glCompileShader(shader.id);
			GLint result = 0xDEADBEEF;
			glGetShaderiv(shader.id, GL_COMPILE_STATUS, &result);

			if (!result)
			{
				m::system::Log err("Shader", m::LOG_ERROR);
				err << "Could not compile: " << shader.filename << " (" << shader.id << ")" << m::endl;
				outputShaderLog(shader.id);
				return false;
			}

			return true;
		}

		void Shader::outputShaderLog(m::u32 shaderID)
		{
			m::i32 length;
			glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &length);
			char* msg = (char*)::malloc(length);
			glGetShaderInfoLog(shaderID, sizeof(msg), &length, msg);

			m::system::Log err("Shader", m::LOG_ERROR);
			err() << "Please validate this shader:" << m::endl;
			err() << msg << m::endl;
		}

		m::u32 Shader::getProgramID() const
		{
			return m_programID;
		}
	}
}
