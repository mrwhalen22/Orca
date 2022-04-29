#include "oapch.h"
#include "Buffer.h"
#include "Renderer.h"

#include "Orca/Platform/OpenGL/OpenGLBuffer.h"

namespace Orca {

	Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size) {
		switch (Renderer::GetAPI()) {
		case RendererAPI::API::None:
			OA_CORE_ASSERT(false, "RendererAPI::None is not Supported!"); 
			return nullptr;
			break;

		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLVertexBuffer>(vertices, size);
			break;

		}
		OA_CORE_ASSERT(false, "No RendererAPI Selected!");
		return nullptr;

	}

	Ref<VertexBuffer> VertexBuffer::Create(uint32_t size) {
		switch (Renderer::GetAPI()) {
		case RendererAPI::API::None:
			OA_CORE_ASSERT(false, "RendererAPI::None is not Supported!");
			return nullptr;
			break;

		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLVertexBuffer>(size);
			break;

		}
		OA_CORE_ASSERT(false, "No RendererAPI Selected!");
		return nullptr;

	}

	Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count) {
		switch (Renderer::GetAPI()) {
		case RendererAPI::API::None:
			OA_CORE_ASSERT(false, "RendererAPI::None is not Supported!");
			return nullptr;
			break;

		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLIndexBuffer>(indices, count);
			break;

		}
		OA_CORE_ASSERT(false, "No RendererAPI Selected!");
		return nullptr;

	}

}