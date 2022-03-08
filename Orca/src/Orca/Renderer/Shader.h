#pragma once
#include "Orca/Core.h"
#include <string>
#include <glm/glm.hpp>

namespace Orca {

	class Shader {

	public:
		Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
		~Shader();

		void Bind();
		void Unbind();

		void UploadUniformMat4(const std::string& name, const glm::mat4& mattrix);
		void UploadUniformFloat4(const std::string& name, const glm::vec4& values);

	private:
		uint32_t m_RendererID;
	};

}