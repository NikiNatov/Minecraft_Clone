#pragma once

#include <glm\glm.hpp>

#include "Core\Base.h"
#include "Graphics\FPSCamera.h"

#include "Graphics\VertexArray.h"
#include "Graphics\Shader.h"
#include "Graphics\Texture2D.h"

struct SceneData
{
	glm::mat4 ProjectionMatrix;
	glm::mat4 ViewMatrix;
	glm::mat4 ViewProjectionMatrix;
	Ref<Shader> ChunkShader;
};

struct RenderCommand
{
	Ref<VertexArray> VAO;
	glm::mat4 Transform;
};

class Renderer
{
public:
	static void Init();

	static void BeginScene(const FPSCamera& camera);
	static void EndScene();
	static void Present();

	static void Submit(const Ref<VertexArray>& vao, const glm::mat4& transform);
	static void SetViewportSize(int x, int y, int width, int height);
	static void ClearScreen(const glm::vec4& color);
private:
	static ScopedPtr<SceneData> s_SceneData;
	static std::vector<RenderCommand> s_RenderCommandQueue;
};