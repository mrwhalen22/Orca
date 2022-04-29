#pragma once

#include "RendererAPI.h"



namespace Orca
{

	class RenderCommand {
	public:

		inline static void Init()
		{
			OA_PROFILE_FUNCTION();
			s_RendererAPI->Init();
		}

		inline static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
			OA_PROFILE_FUNCTION();
			s_RendererAPI->SetViewport(x, y, width, height);
		}

		inline static void DrawIndexed(const Ref<VertexArray>& vertexArray)
		{
			OA_PROFILE_FUNCTION();
			s_RendererAPI->DrawIndexed(vertexArray);
		}


		inline static void Clear()
		{
			OA_PROFILE_FUNCTION();
			s_RendererAPI->Clear();
		}

		inline static void SetClearColor(const glm::vec4& color)
		{
			OA_PROFILE_FUNCTION();
			s_RendererAPI->SetClearColor(color);
		}


	private:
		static RendererAPI* s_RendererAPI;


	};

}