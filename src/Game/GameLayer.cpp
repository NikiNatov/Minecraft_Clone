#include "pch.h"
#include "GameLayer.h"

#include "Core\Base.h"
#include "Graphics\Renderer.h"
#include "Graphics\SpriteManager.h"

GameLayer::GameLayer()
	: m_Camera({10.0f, 60.0f, -20.0}, 45.0f, 16.0f / 9.0f)
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
	m_SpriteSheet = CreateRef<Texture2D>("assets/textures/minecraft_spritesheet.png", TextureFilter::Linear, TextureFilter::Nearest, TextureWrap::ClampToEdge, true);

	// Create all subtextures
	SpriteManager::AddSprite("GrassTop", CreateRef<SubTexture2D>(m_SpriteSheet, 0, 15, 64, 64));
	SpriteManager::AddSprite("GrassSide", CreateRef<SubTexture2D>(m_SpriteSheet, 3, 15, 64, 64));
	SpriteManager::AddSprite("Dirt", CreateRef<SubTexture2D>(m_SpriteSheet, 2, 15, 64, 64));
	SpriteManager::AddSprite("Stone", CreateRef<SubTexture2D>(m_SpriteSheet, 1, 15, 64, 64));
	SpriteManager::AddSprite("Bedrock", CreateRef<SubTexture2D>(m_SpriteSheet, 1, 14, 64, 64));
	SpriteManager::AddSprite("Water", CreateRef<SubTexture2D>(m_SpriteSheet, 13, 3, 64, 64));
	SpriteManager::AddSprite("Sand", CreateRef<SubTexture2D>(m_SpriteSheet, 2, 14, 64, 64));

	// Initialize blocks
	Block::s_GrassBlock = CreateScoped<Block>(BlockID::Grass);
	Block::s_DirtBlock = CreateScoped<Block>(BlockID::Dirt);
	Block::s_StoneBlock = CreateScoped<Block>(BlockID::Stone);
	Block::s_BedrockBlock = CreateScoped<Block>(BlockID::Bedrock);
	Block::s_WaterBlock = CreateScoped<Block>(BlockID::Water);
	Block::s_SandBlock = CreateScoped<Block>(BlockID::Sand);

	// Create chunks
	m_Chunks.reserve(400);

	for (int i = 0; i < 20; i++)
		for(int j = 0 ; j < 20; j++)
			m_Chunks.push_back(CreateRef<Chunk>(glm::vec3(i, 0.0f, j)));
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

	for(auto& chunk : m_Chunks)
		Renderer::Submit(chunk->GetVAO(), m_TextureShader, glm::translate(glm::mat4(1.0f), glm::vec3(chunk->GetGridPosition().x * 16.0f, -63.0f, chunk->GetGridPosition().z * 16.0f)));

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
