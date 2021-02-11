#pragma once

#include <future>

#include "Core\Layer.h"
#include "Core\Events\KeyEvent.h"
#include "Core\Events\WindowEvent.h"
#include "Core\Events\MouseEvent.h"

#include "Graphics\Shader.h"
#include "Graphics\Texture2D.h"

#include "Game\Terrain\World.h"

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
	bool OnKeyPressed(KeyPressedEvent& e);

	void InitializeTextures();
private:
	Ref<Texture2D> m_SpriteSheet;
	Ref<Texture2D> m_CrosshairTexture;
	Ref<Texture2D> m_ItemBoxTexture;

	BlockID m_SelectedBlock = BlockID::Stone;

	Ref<World> m_World;
	float m_MaxBlockBreakDistance = 8.0f;
	float m_MinBlockPlaceDistance = 1.8f;

	std::thread m_ChunkThread;

	bool m_Running = true;
};