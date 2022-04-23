#include "oapch.h"
#include "Renderer2D.h"

#include "VertexArray.h"
#include "Shader.h"
#include "RenderCommand.h"

#include "Orca/Platform/OpenGL/OpenGLShader.h"



namespace Orca {

	struct Renderer2DStorage {
		Ref<VertexArray> VertexArray;
		Ref<Shader> Shader;
	};

	static Renderer2DStorage* s_Data;

	void Renderer2D::Init() {
		s_Data = new Renderer2DStorage();

		float vertices[] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f,
		};



		s_Data->VertexArray = VertexArray::Create();
		Ref<VertexBuffer> squareVB;
		squareVB.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

		{
			BufferLayout layout = {
				{ShaderDataType::Float3, "a_Position"}
			};
			squareVB->SetLayout(layout);
		}


		unsigned int squareIndices[] = { 0, 1, 2, 2, 3, 0 };
		Ref<IndexBuffer> squareIB;
		squareIB.reset(IndexBuffer::Create(squareIndices, 6));

		s_Data->VertexArray->AddVertexBuffer(squareVB);
		s_Data->VertexArray->SetIndexBuffer(squareIB);



		std::string ShaderPath = "assets/shaders/FlatColor.glsl";
		s_Data->Shader = Shader::Create(ShaderPath);
	}

	void Renderer2D::Shutdown() {
		delete s_Data;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera) {
		std::dynamic_pointer_cast<OpenGLShader>(s_Data->Shader)->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(s_Data->Shader)->UploadUniformMat4("u_VPMatrix", camera.GetViewProjectionMatrix());

	
	}
	void Renderer2D::EndScene() {

	}

	// Primitives
	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color) {
		DrawQuad({ position.x, position.y, 0 }, size, color);
	
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color) {
		std::dynamic_pointer_cast<OpenGLShader>(s_Data->Shader)->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(s_Data->Shader)->UploadUniformFloat4("u_Color", color);
		std::dynamic_pointer_cast<OpenGLShader>(s_Data->Shader)->UploadUniformMat4("u_Transform", glm::mat4(1.0f));

		s_Data->VertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->VertexArray);
	}


}