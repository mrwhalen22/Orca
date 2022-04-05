#pragma once
#include "Orca/Renderer/Shader.h"
#include <glm/glm.hpp>

typedef unsigned int GLenum;

namespace Orca {



	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& vertexPath, const std::string& fragmentPath);
		OpenGLShader(const std::string& path);
		virtual ~OpenGLShader();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		void UploadUniformMat3(const std::string& name, const glm::mat3& matrix);
		void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);

		void UploadUniformFloat(const std::string& name, const float value);
		void UploadUniformFloat2(const std::string& name, const glm::vec2& values);
		void UploadUniformFloat3(const std::string& name, const glm::vec3& values);
		void UploadUniformFloat4(const std::string& name, const glm::vec4& values);

		void UploadUniformInt(const std::string& name, const int value);

	private:
		void Compile(std::unordered_map<GLenum, std::string>);
		std::string ReadFile(const std::string& path);
		std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);

	private:
		uint32_t m_RendererID;
	};
};

