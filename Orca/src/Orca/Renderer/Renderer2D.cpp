#include "oapch.h"
#include "Renderer2D.h"

#include "VertexArray.h"
#include "Shader.h"
#include "RenderCommand.h"
#include "SubTexture2D.h"

#include <glm/ext/matrix_transform.hpp>




namespace Orca {

#define DEFAULT_MAX_QUADS 10000

	struct QuadVertex {
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;
		float TexIndex;

	};

	struct Renderer2DData {
		static const uint32_t MaxQuads = DEFAULT_MAX_QUADS;
		static const uint32_t MaxVertices = MaxQuads * 4;
		static const uint32_t MaxIndices = MaxQuads * 6;
		static const uint32_t MaxTextureSlots = 32;

		glm::vec4 QuadVertices[4];


		Ref<VertexArray> VertexArray;
		Ref<VertexBuffer> VertexBuffer;
		Ref<Shader> GenericShader;
		Ref<Texture2D> WhiteTexture;


		uint32_t QuadIndexCount = 0;
		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;

		std::array<Ref<Texture2D>, MaxTextureSlots> QuadTextureSlots;
		uint32_t TextureSlotIndex = 1;

		Renderer2D::Statistics Stats;

	};

	

	static Renderer2DData s_Data;

	void Renderer2D::Init() {
		OA_PROFILE_FUNCTION();
		s_Data.VertexArray = VertexArray::Create();
		
		s_Data.VertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(QuadVertex));
		BufferLayout layout = {
			{ShaderDataType::Float3, "a_Position"},
			{ShaderDataType::Float4, "a_Color"   },
			{ShaderDataType::Float2, "a_TexCoord"},
			{ShaderDataType::Float,  "a_TexIndex"}
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

		int32_t samplers[s_Data.MaxTextureSlots];
		for (int32_t i = 0; i < s_Data.MaxTextureSlots; i++) {
			samplers[i] = i;
		}


		s_Data.GenericShader = Shader::Create("assets/shaders/Renderer2DGeneric.glsl");
		s_Data.GenericShader->Bind();
		s_Data.GenericShader->SetIntArray("u_Textures", samplers, s_Data.MaxTextureSlots);

		s_Data.QuadTextureSlots[0] = s_Data.WhiteTexture;
	
		s_Data.QuadVertices[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.QuadVertices[1] = {  0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.QuadVertices[2] = {  0.5f,  0.5f, 0.0f, 1.0f };
		s_Data.QuadVertices[3] = { -0.5f,  0.5f, 0.0f, 1.0f };

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

		s_Data.TextureSlotIndex = 1;
	}

	void Renderer2D::EndScene() {
		OA_PROFILE_FUNCTION();

		uint32_t dataSize = (uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase;
		s_Data.VertexBuffer->SetData(s_Data.QuadVertexBufferBase, dataSize);

		Flush();
	}

	void Renderer2D::Flush() {
		OA_PROFILE_FUNCTION();
		for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++) {
			s_Data.QuadTextureSlots[i]->Bind(i);
		}
		RenderCommand::DrawIndexed(s_Data.VertexArray, s_Data.QuadIndexCount);
		s_Data.Stats.DrawCalls++;

	}

	void Renderer2D::FlushAndReset() {
		OA_PROFILE_FUNCTION();
		EndScene();
		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

		s_Data.TextureSlotIndex = 1;

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
		DrawQuad(position, size, 0.0f, s_Data.WhiteTexture, color);
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

		if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices) {
			FlushAndReset();
		}

		float textureIndex = 0.0f;
		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++) {
			if (*s_Data.QuadTextureSlots[i].get() == *texture.get()) {
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f) {
			textureIndex = (float)s_Data.TextureSlotIndex;
			s_Data.QuadTextureSlots[s_Data.TextureSlotIndex] = texture;
			s_Data.TextureSlotIndex++;
		}

		glm::mat4 transform = glm::mat4(1.0f);
		
		if (position != glm::vec3(0.0f)) {
			transform *= glm::translate(glm::mat4(1.0f), position);
		}

		if (angle_rads != 0.0f) {
			transform *= glm::rotate(glm::mat4(1.0f), angle_rads, { 0.0f, 0.0f, 1.0f });
		}
		
		if(size != glm::vec2(1.0f)) {
			transform *= glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		} 

		s_Data.QuadVertexBufferPtr->Position = (transform == glm::mat4(1.0f)) ? s_Data.QuadVertices[0] : (transform * s_Data.QuadVertices[0]);
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = {0.0f, 0.0f};
		s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = (transform == glm::mat4(1.0f)) ? s_Data.QuadVertices[1] : (transform * s_Data.QuadVertices[1]);
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f, 0.0f };
		s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = (transform == glm::mat4(1.0f)) ? s_Data.QuadVertices[2] : (transform * s_Data.QuadVertices[2]);
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f, 1.0f };
		s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = (transform == glm::mat4(1.0f)) ? s_Data.QuadVertices[3] : (transform * s_Data.QuadVertices[3]);
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f, 1.0f };
		s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadIndexCount += 6;


		s_Data.Stats.QuadCount++;
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const float angle_rads, const Ref<SubTexture2D>& subtexture, const glm::vec4& color) {
		DrawQuad({ position.x, position.y, 0.0f }, size, angle_rads, subtexture, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const float angle_rads, const Ref<SubTexture2D>& subtexture, const glm::vec4& color) {
		OA_PROFILE_FUNCTION();

		const Ref<Texture2D> texture = subtexture->GetTexture();

		if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices) {
			FlushAndReset();
		}

		float textureIndex = 0.0f;
		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++) {
			if (*s_Data.QuadTextureSlots[i].get() == *texture.get()) {
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f) {
			textureIndex = (float)s_Data.TextureSlotIndex;
			s_Data.QuadTextureSlots[s_Data.TextureSlotIndex] = texture;
			s_Data.TextureSlotIndex++;
		}

		glm::mat4 transform = glm::mat4(1.0f);

		if (position != glm::vec3(0.0f)) {
			transform *= glm::translate(glm::mat4(1.0f), position);
		}

		if (angle_rads != 0.0f) {
			transform *= glm::rotate(glm::mat4(1.0f), angle_rads, { 0.0f, 0.0f, 1.0f });
		}

		if (size != glm::vec2(1.0f)) {
			transform *= glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		}

		const glm::vec2* TexCoords = subtexture->GetTexCoords();

		s_Data.QuadVertexBufferPtr->Position = (transform == glm::mat4(1.0f)) ? s_Data.QuadVertices[0] : (transform * s_Data.QuadVertices[0]);
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = TexCoords[0];
		s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = (transform == glm::mat4(1.0f)) ? s_Data.QuadVertices[1] : (transform * s_Data.QuadVertices[1]);
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = TexCoords[1];
		s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = (transform == glm::mat4(1.0f)) ? s_Data.QuadVertices[2] : (transform * s_Data.QuadVertices[2]);
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = TexCoords[2];
		s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = (transform == glm::mat4(1.0f)) ? s_Data.QuadVertices[3] : (transform * s_Data.QuadVertices[3]);
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = TexCoords[3];
		s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadIndexCount += 6;


		s_Data.Stats.QuadCount++;
	}

	void Renderer2D::DrawQuad(const QuadProps& props) {
		DrawQuad(props.position, props.size, props.rotation_rads, props.texture, props.color);
	}

	void Renderer2D::ResetStats() {
		memset(&s_Data.Stats, 0, sizeof(Renderer2D::Statistics));
	}

	Renderer2D::Statistics Renderer2D::GetStats() {
		return s_Data.Stats;
	}

}