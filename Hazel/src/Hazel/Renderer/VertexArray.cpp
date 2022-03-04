#include "hzpch.h"
#include "VertexArray.h"
#include "Renderer.h"

#include "Hazel/Platform/OpenGL/OpenGLVertexArray.h"

namespace Hazel {

	VertexArray* VertexArray::Create() {
		switch (Renderer::GetAPI()) {
		case RendererAPI::API::None:
			HZ_CORE_ASSERT(false, "RendererAPI::None is not Supported!");
			return nullptr;
			break;

		case RendererAPI::API::OpenGL:
			return new OpenGLVertexArray();
			break;

		}
		HZ_CORE_ASSERT(false, "No RendererAPI Selected!");
		return nullptr;

	}

}