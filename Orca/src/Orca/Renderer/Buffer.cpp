#include "oapch.h"
#include "Buffer.h"
#include "Renderer.h"

#include "Orca/Platform/OpenGL/OpenGLBuffer.h"

namespace Orca {

	VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size) {
		switch (Renderer::GetAPI()) {
		case RendererAPI::API::None:
			OA_CORE_ASSERT(false, "RendererAPI::None is not Supported!"); 
			return nullptr;
			break;

		case RendererAPI::API::OpenGL:
			return new OpenGLVertexBuffer(vertices, size);
			break;

		}
		OA_CORE_ASSERT(false, "No RendererAPI Selected!");
		return nullptr;

	}

	IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t size) {
		switch (Renderer::GetAPI()) {
		case RendererAPI::API::None:
			OA_CORE_ASSERT(false, "RendererAPI::None is not Supported!");
			return nullptr;
			break;

		case RendererAPI::API::OpenGL:
			return new OpenGLIndexBuffer(indices, size);
			break;

		}
		OA_CORE_ASSERT(false, "No RendererAPI Selected!");
		return nullptr;

	}

}