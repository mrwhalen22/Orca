#pragma once
#include "Orca/Core/Core.h"

namespace Orca {

	class Shader {

	public:
		virtual ~Shader() = default;

		virtual void Bind() const  = 0;
		virtual void Unbind() const  = 0;

		static Shader* Create(const std::string& vertexPath, const std::string& fragmentPath);
		static Shader* Create(const std::string& path);
	};

}