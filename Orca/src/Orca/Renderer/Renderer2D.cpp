#include "oapch.h"
#include "Renderer2D.h"

#include "VertexArray.h"
#include "Shader.h"
#include "RenderCommand.h"

#include <glm/ext/matrix_transform.hpp>




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
		s_Data->Shader->Bind();
		s_Data->Shader->SetMat4("u_VPMatrix", camera.GetViewProjectionMatrix());

	
	}
	void Renderer2D::EndScene() {

	}

	// Primitives

	void Renderer2D::DrawQuad(const glm::vec2& position) {
		DrawQuad({ position.x, position.y, 0.0f }, { 1.0f, 1.0f }, 0.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
	}

	void Renderer2D::DrawQuad(const glm::vec3& position) {
		DrawQuad(position, {1.0f, 1.0f}, 0.0f, {1.0f, 1.0f, 1.0f, 1.0f});
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color) {
		DrawQuad(position, size, 0.0f, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color) {
		DrawQuad(position, size, 0.0f, color);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const float angle_rads, const glm::vec4& color) {
		DrawQuad({ position.x, position.y, 0.0f }, size, angle_rads, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const float angle_rads, const glm::vec4& color) {
		s_Data->Shader->Bind();
		s_Data->Shader->SetFloat4("u_Color", color);
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * 
			glm::rotate(glm::mat4(1.0f), angle_rads, { 0.0f, 0.0f, 1.0f }) *
			glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});
		s_Data->Shader->SetMat4("u_Transform", transform);

		s_Data->VertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->VertexArray);
	}


}