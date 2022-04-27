#include "oapch.h"
#include "OpenGLShader.h"

#include <fstream>

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#define MAX_SHADERS 2

namespace Orca {

	int OpenGLShader::GetUniformLocation(const std::string& name) {
		OA_PROFILE_FUNCTION();
		int location;
		if (m_UniformLocations.find(name) == m_UniformLocations.end()) {
			location = glGetUniformLocation(m_RendererID, name.c_str());
			m_UniformLocations[name] = location;
		}
		else location = m_UniformLocations[name];
		return location;
	}

	OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath) 
		: m_Name(name)
	{
		OA_PROFILE_FUNCTION();
		std::string vertexSource = ReadFile(vertexPath);
		std::string fragmentSource = ReadFile(fragmentPath);

		std::unordered_map<GLenum, std::string> sources;
		sources[GL_VERTEX_SHADER] = vertexSource;
		sources[GL_FRAGMENT_SHADER] = fragmentSource;
		Compile(sources);

	}

	OpenGLShader::OpenGLShader(const std::string& path) {
		OA_PROFILE_FUNCTION();
		std::string source = ReadFile(path);
		std::unordered_map<GLenum, std::string> splitSources = PreProcess(source);
		Compile(splitSources);

		auto lastSlash = path.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash+1;

		auto lastDot = path.rfind('.');
		auto count = lastDot == std::string::npos ? path.size() - lastSlash : lastDot - lastSlash;
		m_Name = path.substr(lastSlash, count);
		
	}

	
	OpenGLShader::~OpenGLShader() {
		OA_PROFILE_FUNCTION();
		glDeleteProgram(m_RendererID);

	}

	void OpenGLShader::Bind() const {
		glUseProgram(m_RendererID);

	}

	void OpenGLShader::Unbind() const{
		glUseProgram(0);
	}

	void OpenGLShader::SetMat4(const std::string& name, const glm::mat4& value) {
		UploadUniformMat4(name, value);
	}
	
	void OpenGLShader::SetFloat3(const std::string& name, const glm::vec3& value) {
		UploadUniformFloat3(name, value);
	}
	
	void OpenGLShader::SetFloat4(const std::string& name, const glm::vec4& value) {
		UploadUniformFloat4(name, value);
	}

	void OpenGLShader::SetInt(const std::string& name, const int value) {
		UploadUniformInt(name, value);
	}


	void OpenGLShader::UploadUniformMat3(const std::string& name, const glm::mat3& matrix)
	{
		glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix)
	{
		glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(matrix));
	}


	void OpenGLShader::UploadUniformFloat(const std::string& name, const float value)
	{
		glUniform1f(GetUniformLocation(name), value);
	}

	void OpenGLShader::UploadUniformFloat2(const std::string& name, const glm::vec2& values)
	{
		glUniform2fv(GetUniformLocation(name), 1, glm::value_ptr(values));
	}

	void OpenGLShader::UploadUniformFloat3(const std::string& name, const glm::vec3& values)
	{
		glUniform3fv(GetUniformLocation(name), 1, glm::value_ptr(values));
	}

	void OpenGLShader::UploadUniformFloat4(const std::string& name, const glm::vec4& values)
	{
		glUniform4fv(GetUniformLocation(name), 1, glm::value_ptr(values));
	}


	void OpenGLShader::UploadUniformInt(const std::string& name, const int value)
	{
		glUniform1i(GetUniformLocation(name), value);
	}

	void OpenGLShader::Compile(std::unordered_map<GLenum, std::string> sources) {
		OA_PROFILE_FUNCTION();
		OA_CORE_ASSERT(sources.size() <= MAX_SHADERS, "Too many shaders! Orca supports 2 or less shaders!");
		std::array<GLenum, MAX_SHADERS> glShaderIDs;

		GLuint program = glCreateProgram();

		int shaderIndex = 0;
		for (auto& kv : sources) {
			GLenum type = kv.first;
			const std::string& sourceStr = kv.second;

			GLuint shader = glCreateShader(type);

			const GLchar* source = sourceStr.c_str();
			glShaderSource(shader, 1, &source, 0);

			glCompileShader(shader);

			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

				glDeleteShader(shader);

				OA_CORE_ERROR("{0}", infoLog.data());
				OA_CORE_ASSERT(false, "Shader Compilation Failure:");

				return;
			}

			glAttachShader(program, shader);
			glShaderIDs[shaderIndex++] = shader;
			
		}


		// Link our program
		glLinkProgram(program);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(program);
			// Don't leak shaders either.
			
			for (auto& id : glShaderIDs) {
				glDeleteShader(id);
			}

			// Use the infoLog as you see fit.
			OA_CORE_ERROR("{0}", infoLog.data());
			OA_CORE_ASSERT(false, "Shader Link Failure:");
			// In this simple program, we'll just leave
			return;
		}

		for (auto& id : glShaderIDs) 
			glDetachShader(program, id);

		m_RendererID = program;
	}

	std::string OpenGLShader::ReadFile(const std::string& path) {
		std::string result;
		std::ifstream in(path, std::ios::in | std::ios::binary);
		if (in) {
			in.seekg(0, std::ios::end);
			result.resize(in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(&result[0], result.size());
			in.close();
		}
		else {
			OA_CORE_ERROR("Could not open shader file '{0}'", path);
		}
		return result;
	}

	static GLenum ShaderTypeFromString(const std::string& type) {
		if (type == "vertex") return GL_VERTEX_SHADER;
		else if (type == "fragment" || type == "pixel") return GL_FRAGMENT_SHADER;
		else {
			OA_CORE_ASSERT(false, "Unknown shader type");
		}
		return 0;

	}

	std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& source) {
		OA_PROFILE_FUNCTION();
		std::unordered_map<GLenum, std::string> shaderSources;

		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = source.find(typeToken, 0);

		while (pos != std::string::npos) {
			size_t eol = source.find_first_of("\r\n", pos);
			OA_CORE_ASSERT(eol != std::string::npos, "Syntax error");
			size_t begin = pos + typeTokenLength + 1;
			std::string type = source.substr(begin, eol - begin);
			OA_CORE_ASSERT(ShaderTypeFromString(type), "Invalid shader type specification");

			size_t nextLinePos = source.find_first_not_of("\r\n", eol);
			pos = source.find(typeToken, nextLinePos);
			shaderSources[ShaderTypeFromString(type)] = source.substr(nextLinePos, 
					pos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos));

		}

		return shaderSources;
	}

}