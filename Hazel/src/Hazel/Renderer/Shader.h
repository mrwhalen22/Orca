#pragma once
#include "Hazel/Core.h"
#include <string>

namespace Hazel {

	class Shader {

	public:
		Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
		~Shader();

		void Bind();
		void Unbind();

	private:
		uint32_t m_RendererID;
	};

}