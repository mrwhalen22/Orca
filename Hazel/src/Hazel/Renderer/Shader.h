#pragma once
#include "Hazel/Core.h"
#include <string>
#include <glm/glm.hpp>

namespace Hazel {

	class Shader {

	public:
		Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
		~Shader();

		void Bind();
		void Unbind();

		void UploadUniformMat4(const std::string& name, const glm::mat4& mattrix);

	private:
		uint32_t m_RendererID;
	};

}