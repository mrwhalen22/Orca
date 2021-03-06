#include "oapch.h"
#include "OpenGLVertexArray.h"

#include <glad/glad.h>

namespace Orca {

		static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type) {
			switch (type) {
			case ShaderDataType::None:		return 0;
			case ShaderDataType::Float:		return GL_FLOAT;
			case ShaderDataType::Float2:	return GL_FLOAT;
			case ShaderDataType::Float3:	return GL_FLOAT;
			case ShaderDataType::Float4:	return GL_FLOAT;

			case ShaderDataType::Int:		return GL_INT;
			case ShaderDataType::Int2:		return GL_INT;
			case ShaderDataType::Int3:		return GL_INT;
			case ShaderDataType::Int4:		return GL_INT;

			case ShaderDataType::Mat3:		return GL_FLOAT;
			case ShaderDataType::Mat4:		return GL_FLOAT;
			case ShaderDataType::Bool:		return GL_BOOL;

			}
			OA_CORE_ASSERT(false, "Unknown ShaderDataType!");
			return 0;

		}

		OpenGLVertexArray::OpenGLVertexArray() {
			OA_PROFILE_FUNCTION();
			glCreateVertexArrays(1, &m_RendererID);
		}

		OpenGLVertexArray::~OpenGLVertexArray() {

		}

		void OpenGLVertexArray::Bind() const {
			OA_PROFILE_FUNCTION();
			glBindVertexArray(m_RendererID);
		}

		void OpenGLVertexArray::Unbind() const {
			OA_PROFILE_FUNCTION();
			glBindVertexArray(0);
		}

		void OpenGLVertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vb) {
			OA_PROFILE_FUNCTION();
			glBindVertexArray(m_RendererID);
			vb->Bind();

			OA_CORE_ASSERT(vb->GetLayout().GetElements().size(), "Vertex Buffer has no layout!");

			uint32_t index = 0;
			for (const auto& element : vb->GetLayout()) {
				glEnableVertexAttribArray(index);
				glVertexAttribPointer(index, element.GetComponentCount(),
					ShaderDataTypeToOpenGLBaseType(element.type), element.normalized ? GL_TRUE : GL_FALSE,
					vb->GetLayout().GetStride(), (const void*)element.offset);
				index++;
			}

			m_VertexBuffers.push_back(vb);
			//glBindVertexArray(0);

		}

		void OpenGLVertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& ib) {
			OA_PROFILE_FUNCTION();
			glBindVertexArray(m_RendererID);
			ib->Bind();

			m_IndexBuffer = ib;
		}

	







}