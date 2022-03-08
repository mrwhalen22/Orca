#include "oapch.h"
#include "VertexArray.h"
#include "Renderer.h"

#include "Orca/Platform/OpenGL/OpenGLVertexArray.h"

namespace Orca {

	VertexArray* VertexArray::Create() {
		switch (Renderer::GetAPI()) {
		case RendererAPI::API::None:
			OA_CORE_ASSERT(false, "RendererAPI::None is not Supported!");
			return nullptr;
			break;

		case RendererAPI::API::OpenGL:
			return new OpenGLVertexArray();
			break;

		}
		OA_CORE_ASSERT(false, "No RendererAPI Selected!");
		return nullptr;

	}

}