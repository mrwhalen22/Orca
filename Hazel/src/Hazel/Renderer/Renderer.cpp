#include "hzpch.h"
#include "Hazel/Renderer/Renderer.h"



namespace Hazel {

	Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData;
	

	void Renderer::BeginScene()
	{

	}


	void Renderer::BeginScene(OrthographicCamera& camera)
	{
		
		m_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{


	}


	void Renderer::Submit(const std::shared_ptr<VertexArray>& vertexArray, const std::shared_ptr<Shader>& shader)
	{
		vertexArray->Bind();

		shader->Bind();
		shader->UploadUniformMat4("u_VPMatrix", m_SceneData->ViewProjectionMatrix);

		RenderCommand::DrawIndexed(vertexArray);

	}




}