#include "oapch.h"
#include "Orca/Renderer/RendererAPI.h"
#include "Orca/Renderer/Renderer.h"
#include "Framebuffer.h"
#include "Orca/Platform/OpenGL/OpenGLFramebuffer.h"

namespace Orca {

	Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec) {
		switch (Renderer::GetAPI()) {
		case RendererAPI::API::None:
			OA_CORE_ASSERT(false, "RendererAPI::None is not Supported!");
			return nullptr;
			break;

		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLFramebuffer>(spec);
			break;

		}
		OA_CORE_ASSERT(false, "No RendererAPI Selected!");
		return nullptr;

	}

}