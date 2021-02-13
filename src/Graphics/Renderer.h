#pragma once

#include <glm\glm.hpp>

#include "Core\Base.h"
#include "Graphics\Camera.h"

#include "Graphics\VertexArray.h"
#include "Graphics\Shader.h"
#include "Graphics\Texture2D.h"

#include "Game\Terrain\Chunk.h"

struct SceneData
{
	glm::mat4 ProjectionMatrix;
	glm::mat4 ViewMatrix;
	glm::mat4 ViewProjectionMatrix;
	Ref<Shader> ChunkShader;
};

struct RenderCommand
{
	Ref<Chunk> Chunk;
	glm::mat4 Transform;
};

class Renderer
{
public:
	static void Init();

	static void BeginScene(const Camera& camera, const glm::mat4& transform);
	static void EndScene();
	static void Present();

	static void Submit(const Ref<Chunk>& chunk, const glm::mat4& transform);
	static void SetViewportSize(int x, int y, int width, int height);
	static void ClearScreen(const glm::vec4& color);
private:
	static void PresentSolidMeshes();
	static void PresentTransparentMeshes();
	static void PresentWaterMeshes();
private:
	static ScopedPtr<SceneData> s_SceneData;
	static std::vector<RenderCommand> s_RenderCommandQueue;
};