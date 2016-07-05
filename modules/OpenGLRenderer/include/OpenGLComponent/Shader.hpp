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

#ifndef INCLUDE_ILARGIA_SHADER_HPP
#define INCLUDE_ILARGIA_SHADER_HPP

#include <Muon/String.hpp>
#include <Ilargia/Core/Define.hpp>
#include <gl/gl.h>

#include <unordered_map>

namespace ilg
{
	namespace graphics
	{
		class ILARGIA_API Shader
		{
		public:
			struct GLSLShader
			{
				m::u32 id;
				m::String filename;
				m::String source;
			};

			Shader(const m::String& vertexShader, const m::String& fragmentShader);
			~Shader();

			void unload();
			bool initialize();
			void linkProgram();

			GLuint getUniformLocation(const m::String& name);
			GLuint getAttribLocation(const m::String& name);
			void sendUniform(const m::String& name, const int id);
			void sendUniform4x4(const m::String& name, const m::f32* matrix, bool transpose = false);
			void sendUniform3x3(const m::String& name, const m::f32* matrix, bool transpose = false);
			void sendUniform(const m::String& name, const m::f32 red, const m::f32 green, const m::f32 blue, const m::f32 alpha);
			void sendUniform(const m::String& name, const m::f32 x, const m::f32 y, const m::f32 z);
			void sendUniform(const m::String& name, const m::f32 x, const m::f32 y);
			void sendUniform(const m::String& name, const m::f32 scalar);
			void bindAttrib(m::u32 index, const m::String& attribName);
			void bindShader();

			m::u32 getProgramID() const;

		private:
			m::String readFile(const m::String& filename);
			bool compileShader(const GLSLShader& shader);
			void outputShaderLog(m::u32 shaderID);

			GLSLShader m_vertexShader;
			GLSLShader m_fragmentShader;
			m::u32 m_programID;

			static m::String m_version;
			static bool m_vTest;

			std::unordered_map<m::String, GLuint>* m_uniformMap;
			std::unordered_map<m::String, GLuint>* m_attribMap;

			bool m_initialized;
		};
	}
}

#endif
