#include "pch.h"
#include "Renderer.h"

#include <glad\glad.h>


ScopedPtr<SceneData> Renderer::s_SceneData = CreateScoped<SceneData>();
std::vector<RenderCommand> Renderer::s_RenderCommandQueue;

void Renderer::Init()
{
	s_SceneData->ChunkShader = CreateRef<Shader>("assets/shaders/ChunkShader.glsl");

	glEnable(GL_DEPTH_TEST);
	s_RenderCommandQueue.reserve(1000);
}

void Renderer::BeginScene(const Camera& camera, const glm::mat4& transform)
{
	s_SceneData->ProjectionMatrix = camera.GetProjectionMatrix();
	s_SceneData->ViewMatrix = glm::inverse(transform);
	s_SceneData->ViewProjectionMatrix = s_SceneData->ProjectionMatrix * s_SceneData->ViewMatrix;

	s_RenderCommandQueue.clear();

	s_SceneData->ChunkShader->Bind();

	glDisable(GL_BLEND);
}

void Renderer::EndScene()
{
}

void Renderer::Present()
{
	s_SceneData->ChunkShader->SetMat4("u_ViewProjection", 1, s_SceneData->ViewProjectionMatrix);
	s_SceneData->ChunkShader->SetInt("u_Texture", 0);

	for (auto& command : s_RenderCommandQueue)
	{
		s_SceneData->ChunkShader->SetMat4("u_Transform", 1, command.Transform);
		
		auto& solidMesh = command.Chunk->GetSolidMesh();

		glDisable(GL_BLEND);

		solidMesh->GetVAO()->Bind();
		glDrawElements(GL_TRIANGLES, solidMesh->GetVAO()->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
		solidMesh->GetVAO()->Bind();
	}

	for (auto& command : s_RenderCommandQueue)
	{
		s_SceneData->ChunkShader->SetMat4("u_Transform", 1, command.Transform);

		auto& fluidMesh = command.Chunk->GetFluidMesh();

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		fluidMesh->GetVAO()->Bind();
		glDrawElements(GL_TRIANGLES, fluidMesh->GetVAO()->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
		fluidMesh->GetVAO()->Bind();
	}
}

void Renderer::Submit(const Ref<Chunk>& chunk, const glm::mat4& transform)
{
	RenderCommand command;
	command.Chunk = chunk;
	command.Transform = transform;
	s_RenderCommandQueue.emplace_back(command);
}

void Renderer::SetViewportSize(int x, int y, int width, int height)
{
	glViewport(x, y, width, height);
}

void Renderer::ClearScreen(const glm::vec4& color)
{
	glClearColor(color.r, color.g, color.b, color.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
