#include "oapch.h"
#include "Shader.h"

#include "Orca/Renderer/Renderer.h"
#include "Orca/Platform/OpenGL/OpenGLShader.h"

namespace Orca {

	Shader* Shader::Create(const std::string& vertexPath, const std::string& fragmentPath)
	{
		switch (Renderer::GetAPI()) {
		case RendererAPI::API::None:
			OA_CORE_ASSERT(false, "RendererAPI::None is not Supported!");
			return nullptr;
			break;

		case RendererAPI::API::OpenGL:
			return new OpenGLShader(vertexPath, fragmentPath);
			break;

		}
		OA_CORE_ASSERT(false, "No RendererAPI Selected!");
		return nullptr;

	}

	Shader* Shader::Create(const std::string& path)
	{
		switch (Renderer::GetAPI()) {
		case RendererAPI::API::None:
			OA_CORE_ASSERT(false, "RendererAPI::None is not Supported!");
			return nullptr;
			break;

		case RendererAPI::API::OpenGL:
			return new OpenGLShader(path);
			break;

		}
		OA_CORE_ASSERT(false, "No RendererAPI Selected!");
		return nullptr;

	}



}