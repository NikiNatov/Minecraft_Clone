#include "pch.h"
#include "GameLayer.h"

#include "Core\Base.h"
#include "Graphics\Renderer.h"
#include "Graphics\SpriteManager.h"

GameLayer::GameLayer()
	: m_Camera({10.0f, 0.0f, -20.0}, 45.0f, 16.0f / 9.0f)
{
	
}

GameLayer::~GameLayer()
{
}

void GameLayer::OnAttach()
{
	// Shader
	m_TextureShader = CreateRef<Shader>("assets/shaders/TextureShader.glsl");

	// Sprite sheet
	m_SpriteSheet = CreateRef<Texture2D>("assets/textures/minecraft_spritesheet.png", TextureFilter::Linear, TextureFilter::Nearest, TextureWrap::Repeat, true);

	SpriteManager::AddSprite("GrassTop", CreateRef<SubTexture2D>(m_SpriteSheet, 0, 15, 64, 64));
	SpriteManager::AddSprite("GrassSide", CreateRef<SubTexture2D>(m_SpriteSheet, 3, 15, 64, 64));
	SpriteManager::AddSprite("Dirt", CreateRef<SubTexture2D>(m_SpriteSheet, 2, 15, 64, 64));
	SpriteManager::AddSprite("Stone", CreateRef<SubTexture2D>(m_SpriteSheet, 1, 15, 64, 64));

	Block::s_GrassBlock = CreateScoped<Block>(BlockID::Grass);
	Block::s_DirtBlock = CreateScoped<Block>(BlockID::Dirt);
	Block::s_StoneBlock = CreateScoped<Block>(BlockID::Stone);

	m_Chunk = CreateRef<Chunk>();
	m_Chunk->CreateMesh();
}

void GameLayer::OnDetach()
{
}

void GameLayer::OnUpdate(float dt)
{
	m_Camera.OnUpdate(dt);

	// Render
	Renderer::BeginScene(m_Camera);
	Renderer::ClearScreen({ 0.4f, 0.7f, 1.0f, 1.0f });

	m_SpriteSheet->Bind(0);
	Renderer::Submit(m_Chunk->GetVAO(), m_TextureShader, glm::translate(glm::mat4(1.0f), glm::vec3(0.0, -10.0f, 0.0f)));

	Renderer::EndScene();
	Renderer::Present();
}

void GameLayer::OnEvent(Event& e)
{
	EventDispatcher dispatcher(e);
	dispatcher.Dispatch<WindowResizedEvent>(BIND_FN(GameLayer::OnWindowResized));
}

bool GameLayer::OnWindowResized(WindowResizedEvent& e)
{
	m_Camera.SetProjection(45.0f, (float)e.GetWidth() / (float)e.GetHeight());
	return false;
}
