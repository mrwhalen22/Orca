#include "oapch.h"
#include "Orca/Renderer/Renderer.h"

#include "Orca/Platform/OpenGL/OpenGLShader.h"



namespace Orca {

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


	void Renderer::Submit(const std::shared_ptr<VertexArray>& vertexArray, const std::shared_ptr<Shader>& shader, const glm::mat4& transform)
	{
		vertexArray->Bind();

		shader->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_VPMatrix", m_SceneData->ViewProjectionMatrix);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_Transform", transform);

		RenderCommand::DrawIndexed(vertexArray);

	}




}