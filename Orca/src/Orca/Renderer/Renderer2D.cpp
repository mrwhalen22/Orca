#include "oapch.h"
#include "Renderer2D.h"

#include "VertexArray.h"
#include "Shader.h"
#include "RenderCommand.h"

#include <glm/ext/matrix_transform.hpp>




namespace Orca {

	struct QuadVertex {
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;

	};

	struct Renderer2DData {
		const uint32_t MaxQuads = 10000;
		const uint32_t MaxVertices = MaxQuads * 4;
		const uint32_t MaxIndices = MaxQuads * 6;

		Ref<VertexArray> VertexArray;
		Ref<VertexBuffer> VertexBuffer;
		Ref<Shader> GenericShader;
		Ref<Texture2D> WhiteTexture;

		uint32_t QuadIndexCount = 0;
		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;
	};

	

	static Renderer2DData s_Data;

	void Renderer2D::Init() {
		OA_PROFILE_FUNCTION();
		s_Data.VertexArray = VertexArray::Create();
		
		s_Data.VertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(QuadVertex));
		BufferLayout layout = {
			{ShaderDataType::Float3, "a_Position"},
			{ShaderDataType::Float4, "a_Color"   },
			{ShaderDataType::Float2, "a_TexCoord"}
		};
		s_Data.VertexBuffer->SetLayout(layout);
		s_Data.VertexArray->AddVertexBuffer(s_Data.VertexBuffer);

		s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.MaxVertices];

		uint32_t* quadIndices = new uint32_t[s_Data.MaxIndices];

		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_Data.MaxIndices; i+=6) {
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;
			offset += 4;
		}

		Ref<IndexBuffer> quadIB = IndexBuffer::Create(quadIndices, s_Data.MaxIndices);
		s_Data.VertexArray->SetIndexBuffer(quadIB);
		delete[] quadIndices;


		s_Data.WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_Data.WhiteTexture->SetData(&whiteTextureData, sizeof(whiteTextureData));

		s_Data.GenericShader = Shader::Create("assets/shaders/Renderer2DGeneric.glsl");
		s_Data.GenericShader->SetInt("u_Texture", 0);
	}

	void Renderer2D::Shutdown() {
		OA_PROFILE_FUNCTION();
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera) {
		OA_PROFILE_FUNCTION();
		s_Data.GenericShader->Bind();
		s_Data.GenericShader->SetMat4("u_VPMatrix", camera.GetViewProjectionMatrix());

		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;
	}
	void Renderer2D::EndScene() {
		OA_PROFILE_FUNCTION();

		uint32_t dataSize = (uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase;
		s_Data.VertexBuffer->SetData(s_Data.QuadVertexBufferBase, dataSize);

		Flush();
	}

	void Renderer2D::Flush() {
		RenderCommand::DrawIndexed(s_Data.VertexArray, s_Data.QuadIndexCount);

	}

	// Primitives

	void Renderer2D::DrawQuad(const glm::vec2& position) {
		DrawQuad({ position.x, position.y, 0.0f }, { 1.0f,1.0f }, 0.0f, (s_Data.WhiteTexture), { 1.0f,1.0f,1.0f,1.0f });
	}

	void Renderer2D::DrawQuad(const glm::vec3& position) {
		DrawQuad(position, { 1.0f,1.0f }, 0.0f, (s_Data.WhiteTexture), { 1.0f,1.0f,1.0f,1.0f });
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color) {
		DrawQuad({ position.x, position.y, 0.0f }, size, 0.0f, (s_Data.WhiteTexture), color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color) {
		OA_PROFILE_FUNCTION();

		s_Data.QuadVertexBufferPtr->Position = position;
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f, 0.0f };
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = { position.x + size.x, position.y, 0.0f };
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f, 0.0f };
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = { position.x + size.x, position.y + size.y, 0.0f };
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f, 1.0f };
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = { position.x, position.y + size.y, 0.0f };
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f, 1.0f };
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadIndexCount += 6;
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const float angle_rads, const glm::vec4& color) {
		DrawQuad({ position.x, position.y, 0.0f }, size, angle_rads, (s_Data.WhiteTexture), color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const float angle_rads, const glm::vec4& color) {
		DrawQuad(position, size, angle_rads, (s_Data.WhiteTexture), color);
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

		s_Data.QuadVertexBufferPtr->Position = position;
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = {0.0f, 0.0f};
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = { position.x + size.x, position.y, 0.0f };
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f, 0.0f };
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = { position.x + size.x, position.y + size.y, 0.0f };
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f, 1.0f };
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = { position.x, position.y + size.y, 0.0f };
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f, 1.0f };
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadIndexCount += 6;

		/*s_Data.GenericShader->Bind();
		s_Data.GenericShader->SetFloat("u_TilingFactor", 1.0f);

		texture->Bind();

		if (angle_rads == 0.0f) {
			glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
				glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
			s_Data.GenericShader->SetMat4("u_Transform", transform);
		}
		else {
			glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
				glm::rotate(glm::mat4(1.0f), angle_rads, { 0.0f, 0.0f, 1.0f }) *
				glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f });
			s_Data.GenericShader->SetMat4("u_Transform", transform);
		}
		
		s_Data.VertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data.VertexArray);*/
	}

	void Renderer2D::DrawQuad(const QuadProps& props) {
		OA_PROFILE_FUNCTION();
		
		// set uniforms
		s_Data.GenericShader->Bind();
		s_Data.GenericShader->SetFloat4("u_Color", props.color);
		s_Data.GenericShader->SetFloat("u_TilingFactor", props.tilingFactor);

		// check and bind texture (white if not specified)
		if (props.texture)
			props.texture->Bind();
		else
			s_Data.WhiteTexture->Bind();

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
		s_Data.GenericShader->SetMat4("u_Transform", transform);

		// send the draw call
		s_Data.VertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data.VertexArray);
	}



}