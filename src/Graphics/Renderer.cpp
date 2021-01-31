#include "pch.h"
#include "Renderer.h"

#include <glad\glad.h>

ScopedPtr<SceneData> Renderer::s_SceneData = CreateScoped<SceneData>();
std::vector<RenderCommand> Renderer::s_RenderCommandQueue;

void Renderer::Init()
{
	s_RenderCommandQueue.reserve(1000);
}

void Renderer::BeginScene(const FPSCamera& camera)
{
	s_SceneData->ProjectionMatrix = camera.GetProjectionMatrix();
	s_SceneData->ViewMatrix = camera.GetViewMatrix();
	s_SceneData->ViewProjectionMatrix = s_SceneData->ProjectionMatrix * s_SceneData->ViewMatrix;

	s_RenderCommandQueue.clear();
}

void Renderer::EndScene()
{
}

void Renderer::Present()
{
	for (auto& command : s_RenderCommandQueue)
	{
		command.Shader->Bind();
		command.Shader->SetMat4("u_ViewProjection", 1, s_SceneData->ViewProjectionMatrix);
		command.Shader->SetMat4("u_Transform", 1, command.Transform);
		command.Texture->Bind(0);
		command.VAO->Bind();
		glDrawElements(GL_TRIANGLES, command.VAO->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
	}
}

void Renderer::Submit(const Ref<VertexArray>& vao, const Ref<Shader>& shader, const Ref<Texture2D>& texture, const glm::mat4& transform)
{
	RenderCommand command;
	command.VAO = vao;
	command.Shader = shader;
	command.Transform = transform;
	command.Texture = texture;
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
