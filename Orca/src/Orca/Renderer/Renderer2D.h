#pragma once
#include "OrthographicCamera.h"
#include "Texture.h"
#include "SubTexture2D.h"

namespace Orca {

	typedef struct QuadProps {
		glm::vec3 position = {0.0f, 0.0f, 0.0f};
		glm::vec2 size = {1.0f, 1.0f};
		float rotation_rads = 0.0f;
		Ref<Texture2D> texture;
		glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
		float tilingFactor = 1.0f;
	} QuadProps;


	class  Renderer2D {
	
	public:

		static void Init();
		static void Shutdown();

		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();
		static void Flush();

		// Primitives
		static void DrawQuad(const glm::vec2& position);
		static void DrawQuad(const glm::vec3& position);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const float angle_rads, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const float angle_rads, const glm::vec4& color);

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const float angle_rads, const Ref<Texture2D>& texture);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const float angle_rads, const Ref<Texture2D>& texture);

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const float angle_rads, const Ref<Texture2D>& texture, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const float angle_rads, const Ref<Texture2D>& texture, const glm::vec4& color);

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const float angle_rads, const Ref<SubTexture2D>& texture, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const float angle_rads, const Ref<SubTexture2D>& texture, const glm::vec4& color);

		static void DrawQuad(const QuadProps& props);

		struct Statistics {
			uint32_t DrawCalls = 0;
			uint32_t QuadCount = 0;

			uint32_t GetTotalVertexCount() { return QuadCount * 4; }
			uint32_t GetTotalIndexCount() { return QuadCount * 6; }
		};

		static void ResetStats();
		static Statistics GetStats();

	private:
		static void FlushAndReset();
	};

}