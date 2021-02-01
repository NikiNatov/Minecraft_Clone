#pragma once

#include "Core\Layer.h"
#include "Core\Events\KeyEvent.h"
#include "Core\Events\WindowEvent.h"

#include "Graphics\Shader.h"
#include "Graphics\Texture2D.h"
#include "Graphics\FPSCamera.h"
#include "Game\Terrain\Chunk.h"

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
private:
	Ref<Shader> m_TextureShader;
	Ref<Texture2D> m_SpriteSheet;
	std::vector<Ref<Chunk>> m_Chunks;
	FPSCamera m_Camera;
};