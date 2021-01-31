#pragma once

#include "Core\Layer.h"
#include "Core\Events\KeyEvent.h"
#include "Core\Events\WindowEvent.h"

#include "Graphics\Shader.h"
#include "Graphics\Texture2D.h"
#include "Graphics\FPSCamera.h"
#include "Game\Chunk.h"

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
	Ref<Chunk> m_Chunk;
	FPSCamera m_Camera;
};