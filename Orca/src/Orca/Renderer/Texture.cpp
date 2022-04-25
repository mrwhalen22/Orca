#include "oapch.h"
#include "Texture.h"
#include "Orca/Renderer/Renderer.h"

#include "Orca/Platform/OpenGL/OpenGLTexture.h"

namespace Orca
{
	Ref<Texture2D> Texture2D::Create(const std::string& path)
	{
		switch (Renderer::GetAPI()) {
		case RendererAPI::API::None:
			OA_CORE_ASSERT(false, "RendererAPI::None is not Supported!");
			return nullptr;
			break;

		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLTexture2D>(path);
			break;

		}
		OA_CORE_ASSERT(false, "No RendererAPI Selected!");
		return nullptr;

	}

	Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height)
	{
		switch (Renderer::GetAPI()) {
		case RendererAPI::API::None:
			OA_CORE_ASSERT(false, "RendererAPI::None is not Supported!");
			return nullptr;
			break;

		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLTexture2D>(width, height);
			break;

		}
		OA_CORE_ASSERT(false, "No RendererAPI Selected!");
		return nullptr;

	}


}
