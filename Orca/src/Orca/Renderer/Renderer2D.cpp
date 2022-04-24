#include "oapch.h"
#include "Renderer2D.h"

#include "VertexArray.h"
#include "Shader.h"
#include "RenderCommand.h"

#include <glm/ext/matrix_transform.hpp>




namespace Orca {

	struct Renderer2DStorage {
		Ref<VertexArray> VertexArray;
		Ref<Shader> FlatShader;
		Ref<Shader> TextureShader;
	};

	static Renderer2DStorage* s_Data;

	void Renderer2D::Init() {
		s_Data = new Renderer2DStorage();

		float vertices[] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};



		s_Data->VertexArray = VertexArray::Create();
		Ref<VertexBuffer> squareVB;
		squareVB.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

		{
			BufferLayout layout = {
				{ShaderDataType::Float3, "a_Position"},
				{ShaderDataType::Float2, "a_TexCoord"}
			};
			squareVB->SetLayout(layout);
		}


		unsigned int squareIndices[] = { 0, 1, 2, 2, 3, 0 };
		Ref<IndexBuffer> squareIB;
		squareIB.reset(IndexBuffer::Create(squareIndices, 6));

		s_Data->VertexArray->AddVertexBuffer(squareVB);
		s_Data->VertexArray->SetIndexBuffer(squareIB);



		std::string FlatShaderPath = "assets/shaders/FlatColor.glsl";
		s_Data->FlatShader = Shader::Create(FlatShaderPath);

		std::string TextureShaderPath = "assets/shaders/Texture.glsl";
		s_Data->TextureShader = Shader::Create(TextureShaderPath);
		s_Data->TextureShader->SetInt("u_Texture", 0);
	}

	void Renderer2D::Shutdown() {
		delete s_Data;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera) {
		s_Data->FlatShader->Bind();
		s_Data->FlatShader->SetMat4("u_VPMatrix", camera.GetViewProjectionMatrix());

		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetMat4("u_VPMatrix", camera.GetViewProjectionMatrix());

	
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
		s_Data->FlatShader->Bind();
		s_Data->FlatShader->SetFloat4("u_Color", color);
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * 
			glm::rotate(glm::mat4(1.0f), angle_rads, { 0.0f, 0.0f, 1.0f }) *
			glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});
		s_Data->FlatShader->SetMat4("u_Transform", transform);

		s_Data->VertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->VertexArray);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const float angle_rads, const Ref<Texture2D>& texture) {
		DrawQuad({ position.x, position.y, 0.0f }, size, angle_rads, texture);
	}
	
	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const float angle_rads, const Ref<Texture2D>& texture) {
		s_Data->TextureShader->Bind();
		
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::rotate(glm::mat4(1.0f), angle_rads, { 0.0f, 0.0f, 1.0f }) *
			glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_Data->TextureShader->SetMat4("u_Transform", transform);

		texture->Bind();
		s_Data->VertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->VertexArray);
	}



}