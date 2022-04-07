#include "oapch.h"
#include "Shader.h"

#include "Orca/Renderer/Renderer.h"
#include "Orca/Platform/OpenGL/OpenGLShader.h"

namespace Orca {

	Ref<Shader> Shader::Create(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath)
	{
		switch (Renderer::GetAPI()) {
		case RendererAPI::API::None:
			OA_CORE_ASSERT(false, "RendererAPI::None is not Supported!");
			return nullptr;
			break;

		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLShader>(name, vertexPath, fragmentPath);
			break;

		}
		OA_CORE_ASSERT(false, "No RendererAPI Selected!");
		return nullptr;

	}

	Ref<Shader> Shader::Create(const std::string& path)
	{
		switch (Renderer::GetAPI()) {
		case RendererAPI::API::None:
			OA_CORE_ASSERT(false, "RendererAPI::None is not Supported!");
			return nullptr;
			break;

		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLShader>(path);
			break;

		}
		OA_CORE_ASSERT(false, "No RendererAPI Selected!");
		return nullptr;

	}

	void ShaderLibrary::Add(const std::string& name, const Ref<Shader>& shader) {
		OA_CORE_ASSERT(m_Shaders.find(name) == m_Shaders.end(), "Shader already exists in Shader Library!");
		m_Shaders[name] = shader;
	}

	void ShaderLibrary::Add(const Ref<Shader>& shader) {
		auto& name = shader->GetName();
		Add(name, shader);
	}
	

	Ref<Shader> ShaderLibrary::Load(const std::string& filepath) {
		auto shader = Shader::Create(filepath);
		Add(shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::Load(const std::string& name, const std::string& filepath) {
		auto shader = Shader::Create(filepath);
		Add(name, shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::Get(const std::string& name) {
		OA_CORE_ASSERT(m_Shaders.find(name) != m_Shaders.end(), "Shader is not in the Shader Lbrary!");
		return m_Shaders[name];
	}



}