#pragma once

#include <future>

#include "Core\Layer.h"
#include "Core\Events\KeyEvent.h"
#include "Core\Events\WindowEvent.h"
#include "Core\Events\MouseEvent.h"

#include "Graphics\Shader.h"
#include "Graphics\Texture2D.h"
#include "Graphics\FPSCamera.h"
#include "Game\Terrain\Chunk.h"
#include "Utils\Math.h"

class GameLayer : public Layer
{
public:
	GameLayer();
	~GameLayer();

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void OnUpdate(float dt) override;
	virtual void OnEvent(Event& e) override;

private:
	bool OnWindowResized(WindowResizedEvent& e);
	bool OnMouseButtonClicked(MouseButtonPressedEvent& e);

	void LoadChunks();
	void InitializeTextures();
private:
	Ref<Texture2D> m_SpriteSheet;
	Ref<Texture2D> m_CrosshairTexture;
	Ref<Texture2D> m_ItemBoxTexture;
	std::unordered_map<glm::vec2, Ref<Chunk>, Math::Vec2Hash> m_ChunkMap;
	FPSCamera m_Camera;
	glm::ivec2 m_LastChunkPos = { 0, 0 };

	int m_ChunkRenderDistance = 8;
	float m_BreakDistance = 8.0f;

	std::mutex m_Mutex;
	std::thread m_ChunkThread;

	bool m_Running = true;
};