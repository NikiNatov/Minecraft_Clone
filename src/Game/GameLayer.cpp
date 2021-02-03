#include "pch.h"
#include "GameLayer.h"

#include "Core\Base.h"
#include "Graphics\Renderer.h"
#include "Graphics\SpriteManager.h"


GameLayer::GameLayer()
	: m_Camera({0.0f, 10.0f, 0.0}, 45.0f, 16.0f / 9.0f)
{
}

GameLayer::~GameLayer()
{
	m_Running = false;
	m_ChunkThread.join();
}

void GameLayer::OnAttach()
{
	// Shader
	m_ChunkShader = CreateRef<Shader>("assets/shaders/ChunkShader.glsl");

	// Sprite sheet
	m_SpriteSheet = CreateRef<Texture2D>("assets/textures/minecraft_spritesheet.png", TextureFilter::MipmapLinear, TextureFilter::Nearest, TextureWrap::ClampToEdge, true);

	// Create all subtextures
	SpriteManager::AddSprite("GrassTop", CreateRef<SubTexture2D>(m_SpriteSheet, 0, 15, 64, 64));
	SpriteManager::AddSprite("GrassSide", CreateRef<SubTexture2D>(m_SpriteSheet, 3, 15, 64, 64));
	SpriteManager::AddSprite("Dirt", CreateRef<SubTexture2D>(m_SpriteSheet, 2, 15, 64, 64));
	SpriteManager::AddSprite("Stone", CreateRef<SubTexture2D>(m_SpriteSheet, 1, 15, 64, 64));
	SpriteManager::AddSprite("Bedrock", CreateRef<SubTexture2D>(m_SpriteSheet, 1, 14, 64, 64));
	SpriteManager::AddSprite("Water", CreateRef<SubTexture2D>(m_SpriteSheet, 13, 3, 64, 64));
	SpriteManager::AddSprite("Sand", CreateRef<SubTexture2D>(m_SpriteSheet, 2, 14, 64, 64));
	SpriteManager::AddSprite("Wood", CreateRef<SubTexture2D>(m_SpriteSheet, 4, 8, 64, 64));
	SpriteManager::AddSprite("Leaf", CreateRef<SubTexture2D>(m_SpriteSheet, 4, 12, 64, 64));

	// Initialize blocks
	Block::s_GrassBlock = CreateScoped<Block>(BlockID::Grass);
	Block::s_DirtBlock = CreateScoped<Block>(BlockID::Dirt);
	Block::s_StoneBlock = CreateScoped<Block>(BlockID::Stone);
	Block::s_BedrockBlock = CreateScoped<Block>(BlockID::Bedrock);
	Block::s_WaterBlock = CreateScoped<Block>(BlockID::Water);
	Block::s_SandBlock = CreateScoped<Block>(BlockID::Sand);
	Block::s_WoodBlock = CreateScoped<Block>(BlockID::Wood);
	Block::s_LeafBlock = CreateScoped<Block>(BlockID::Leaf);

	// Create chunks
	
	m_ChunkThread = std::thread([&]()
	{
		LoadChunks();
	});
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

	for (auto [position, chunk] : m_ChunkMap)
	{
		if(chunk && !chunk->GetVAO())
			chunk->InitializeVAO();

		Renderer::Submit(chunk->GetVAO(), m_ChunkShader, glm::translate(glm::mat4(1.0f), glm::vec3(chunk->GetGridPosition().x * 16.0f, -63.0f, chunk->GetGridPosition().z * 16.0f)));
	}

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

void GameLayer::LoadChunks()
{
	while (m_Running)
	{
		int currentChunkX = m_Camera.GetPosition().x / Chunk::WIDTH;
		int currentChunkZ = m_Camera.GetPosition().z / Chunk::DEPTH;

		for (auto it = m_ChunkMap.begin(); it != m_ChunkMap.end();)
		{
			if (it->first.x > currentChunkX + m_ChunkRenderDistance || it->first.x < currentChunkX - m_ChunkRenderDistance ||
				it->first.y > currentChunkZ + m_ChunkRenderDistance || it->first.y < currentChunkZ - m_ChunkRenderDistance)
			{
				std::lock_guard<std::mutex> lock(m_Mutex);
				it = m_ChunkMap.erase(it);
				continue;
			}
			else
				it++;
		}

		for (int i = 0; i < m_ChunkRenderDistance; i++)
		{
			for (int x = -i; x <= i; x++)
			{
				for (int z = -i; z <= i; z++)
				{
					if (m_ChunkMap.find({ currentChunkX + x, currentChunkZ + z }) == m_ChunkMap.end())
					{
						std::lock_guard<std::mutex> lock(m_Mutex);
						m_ChunkMap[{ currentChunkX + x, currentChunkZ + z }] = CreateRef<Chunk>(glm::vec3(currentChunkX + x, 0, currentChunkZ + z));
					}
				}
			}
		}
	}
}