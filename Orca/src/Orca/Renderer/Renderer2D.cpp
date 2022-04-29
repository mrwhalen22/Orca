#include "oapch.h"
#include "Renderer2D.h"

#include "VertexArray.h"
#include "Shader.h"
#include "RenderCommand.h"

#include <glm/ext/matrix_transform.hpp>




namespace Orca {

	struct Renderer2DStorage {
		Ref<VertexArray> VertexArray;
		Ref<Shader> GenericShader;
		Ref<Texture2D> WhiteTexture;
	};

	static Renderer2DStorage* s_Data;

	void Renderer2D::Init() {
		OA_PROFILE_FUNCTION();
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

		s_Data->WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_Data->WhiteTexture->SetData(&whiteTextureData, sizeof(whiteTextureData));

		s_Data->GenericShader = Shader::Create("assets/shaders/Renderer2DGeneric.glsl");
		s_Data->GenericShader->SetInt("u_Texture", 0);
	}

	void Renderer2D::Shutdown() {
		OA_PROFILE_FUNCTION();
		delete s_Data;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera) {
		OA_PROFILE_FUNCTION();
		s_Data->GenericShader->Bind();
		s_Data->GenericShader->SetMat4("u_VPMatrix", camera.GetViewProjectionMatrix());

	
	}
	void Renderer2D::EndScene() {
		OA_PROFILE_FUNCTION();
	}

	// Primitives

	void Renderer2D::DrawQuad(const glm::vec2& position) {
		DrawQuad({ position.x, position.y, 0.0f }, { 1.0f,1.0f }, 0.0f, (s_Data->WhiteTexture), { 1.0f,1.0f,1.0f,1.0f });
	}

	void Renderer2D::DrawQuad(const glm::vec3& position) {
		DrawQuad(position, { 1.0f,1.0f }, 0.0f, (s_Data->WhiteTexture), { 1.0f,1.0f,1.0f,1.0f });
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color) {
		DrawQuad({ position.x, position.y, 0.0f }, size, 0.0f, (s_Data->WhiteTexture), color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color) {
		DrawQuad(position, size, 0.0f, (s_Data->WhiteTexture), color);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const float angle_rads, const glm::vec4& color) {
		DrawQuad({ position.x, position.y, 0.0f }, size, angle_rads, (s_Data->WhiteTexture), color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const float angle_rads, const glm::vec4& color) {
		DrawQuad(position, size, angle_rads, (s_Data->WhiteTexture), color);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const float angle_rads, const Ref<Texture2D>& texture) {
		DrawQuad({ position.x, position.y, 0.0f }, size, angle_rads, texture, { 1.0f,1.0f,1.0f,1.0f });
	}
	
	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const float angle_rads, const Ref<Texture2D>& texture) {
		DrawQuad(position, size, angle_rads, texture, { 1.0f,1.0f,1.0f,1.0f });
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const float angle_rads, const Ref<Texture2D>& texture, const glm::vec4& color) {
		DrawQuad({ position.x, position.y, 0.0f }, size, angle_rads, texture, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const float angle_rads, const Ref<Texture2D>& texture, const glm::vec4& color) {
		OA_PROFILE_FUNCTION();
		s_Data->GenericShader->Bind();
		s_Data->GenericShader->SetFloat4("u_Color", color);
		s_Data->GenericShader->SetFloat("u_TilingFactor", 1.0f);

		texture->Bind();

		if (angle_rads == 0.0f) {
			glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
				glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
			s_Data->GenericShader->SetMat4("u_Transform", transform);
		}
		else {
			glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
				glm::rotate(glm::mat4(1.0f), angle_rads, { 0.0f, 0.0f, 1.0f }) *
				glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f });
			s_Data->GenericShader->SetMat4("u_Transform", transform);
		}
		
		s_Data->VertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->VertexArray);
	}

	void Renderer2D::DrawQuad(const QuadProps props) {
		OA_PROFILE_FUNCTION();
		
		// set uniforms
		s_Data->GenericShader->Bind();
		s_Data->GenericShader->SetFloat4("u_Color", props.color);
		s_Data->GenericShader->SetFloat("u_TilingFactor", props.tilingFactor);

		// check and bind texture (white if not specified)
		if (props.texture)
			props.texture->Bind();
		else
			s_Data->WhiteTexture->Bind();

		// set the transform -- if no rotation do not calculate it
		glm::mat4 transform;
		if (props.rotation_rads == 0.0f) {
			transform = glm::translate(glm::mat4(1.0f), props.position) *
				glm::scale(glm::mat4(1.0f), { props.size.x, props.size.y, 1.0f });
		}
		else {
			transform = glm::translate(glm::mat4(1.0f), props.position) *
				glm::rotate(glm::mat4(1.0f), props.rotation_rads, { 0.0f, 0.0f, 1.0f }) *
				glm::scale(glm::mat4(1.0f), { props.size.x, props.size.y, 1.0f });
		}
		
		// set transform
		s_Data->GenericShader->SetMat4("u_Transform", transform);

		// send the draw call
		s_Data->VertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->VertexArray);
	}



}