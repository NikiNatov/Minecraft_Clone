#include "pch.h"
#include "GameLayer.h"

#include "Core\Base.h"
#include "Graphics\Renderer.h"
#include "Graphics\Renderer2D.h"
#include "Graphics\SpriteManager.h"

#include "Core\Input.h"
#include "Core\MouseCodes.h"
#include "Core\KeyCodes.h"

#pragma region Constructors / Destructors

GameLayer::GameLayer()
	: m_Camera({0.0f, 60.0f, 0.0}, 45.0f, 16.0f / 9.0f)
{
}

GameLayer::~GameLayer()
{
	m_Running = false;
	m_ChunkThread.join();
}

#pragma endregion

#pragma region Methods

void GameLayer::OnAttach()
{
	// Initialize textures
	InitializeTextures();

	// Initialize blocks
	Block::CreateBlockTemplates();

	// Chunk update thread
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

	// Render Chunks
	Renderer::BeginScene(m_Camera);
	Renderer::ClearScreen({ 0.4f, 0.7f, 1.0f, 1.0f });

	m_SpriteSheet->Bind(0);

	for (auto it = m_ChunkMap.begin(); it != m_ChunkMap.end(); it++)
	{
		Ref<Chunk> chunk(it->second);

		if (!chunk->GetVAO())
			chunk->InitializeVAO();

		Renderer::Submit(chunk->GetVAO(), glm::translate(glm::mat4(1.0f), glm::vec3(chunk->GetGridPosition().x * 16.0f, 0.0f, chunk->GetGridPosition().z * 16.0f)));
	}

	Renderer::EndScene();
	Renderer::Present();

	// Render UI
	// TODO: move into separate layer
	Renderer2D::BeginScene(glm::ortho(-16.0f, 16.0f, 9.0f, -9.0f, 0.0f, 1.0f));
	Renderer2D::DrawQuad(glm::mat4(1.0f), m_CrosshairTexture);
	Renderer2D::DrawQuad(glm::translate(glm::mat4(1.0f), glm::vec3(-4.0f, 7.5f, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(0.6f)), SpriteManager::GetSprite("GrassSide"));
	Renderer2D::DrawQuad(glm::translate(glm::mat4(1.0f), glm::vec3(-4.0f, 7.5f, 0.0f)), m_ItemBoxTexture);

	Renderer2D::DrawQuad(glm::translate(glm::mat4(1.0f), glm::vec3(-2.66f, 7.5f, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(0.6f)), SpriteManager::GetSprite("Stone"));
	Renderer2D::DrawQuad(glm::translate(glm::mat4(1.0f), glm::vec3(-2.66f, 7.5f, 0.0f)), m_ItemBoxTexture);

	Renderer2D::DrawQuad(glm::translate(glm::mat4(1.0f), glm::vec3(-1.33f, 7.5f, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(0.6f)), SpriteManager::GetSprite("Dirt"));
	Renderer2D::DrawQuad(glm::translate(glm::mat4(1.0f), glm::vec3(-1.33f, 7.5f, 0.0f)), m_ItemBoxTexture);

	Renderer2D::DrawQuad(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 7.5f, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(0.6f)), SpriteManager::GetSprite("Sand"));
	Renderer2D::DrawQuad(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 7.5f, 0.0f)), m_ItemBoxTexture);

	Renderer2D::DrawQuad(glm::translate(glm::mat4(1.0f), glm::vec3(1.33f, 7.5f, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(0.6f)), SpriteManager::GetSprite("Wood"));
	Renderer2D::DrawQuad(glm::translate(glm::mat4(1.0f), glm::vec3(1.33f, 7.5f, 0.0f)), m_ItemBoxTexture);

	Renderer2D::DrawQuad(glm::translate(glm::mat4(1.0f), glm::vec3(2.66f, 7.5f, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(0.6f)), SpriteManager::GetSprite("Plank"));
	Renderer2D::DrawQuad(glm::translate(glm::mat4(1.0f), glm::vec3(2.66f, 7.5f, 0.0f)), m_ItemBoxTexture);

	Renderer2D::DrawQuad(glm::translate(glm::mat4(1.0f), glm::vec3(4.0f, 7.5f, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(0.6f)), SpriteManager::GetSprite("Glass"));
	Renderer2D::DrawQuad(glm::translate(glm::mat4(1.0f), glm::vec3(4.0f, 7.5f, 0.0f)), m_ItemBoxTexture);
}

#pragma endregion

#pragma region Events

void GameLayer::OnEvent(Event& e)
{
	EventDispatcher dispatcher(e);
	dispatcher.Dispatch<WindowResizedEvent>(BIND_FN(GameLayer::OnWindowResized));
	dispatcher.Dispatch<MouseButtonPressedEvent>(BIND_FN(GameLayer::OnMouseButtonClicked));
	dispatcher.Dispatch<KeyPressedEvent>(BIND_FN(GameLayer::OnKeyPressed));
}

bool GameLayer::OnWindowResized(WindowResizedEvent& e)
{
	m_Camera.SetProjection(45.0f, (float)e.GetWidth() / (float)e.GetHeight());
	return false;
}

bool GameLayer::OnMouseButtonClicked(MouseButtonPressedEvent& e)
{
	auto intersected = Math::CastRayAndGetIntersectingBlocks(m_Camera.GetPosition(), m_Camera.GetCameraFront(), m_BreakDistance);

	glm::vec3 prevPosition = intersected.front();

	for (const auto& currentPosition : intersected)
	{
		glm::ivec3 localPos = Math::GetBlockPositionInChunk(currentPosition);
		glm::ivec2 chunkPos = Math::GetChunkPositionFromWorldPosition(currentPosition);

		Ref<Chunk> chunk = m_ChunkMap[chunkPos];
		BlockID block = (chunk->GetBlock(localPos.x, localPos.y, localPos.z));

		if (block != BlockID::Air && block != BlockID::Water)
		{
			if (e.GetButton() == Mouse::ButtonLeft)
			{
				chunk->SetBlock(localPos.x, localPos.y, localPos.z, BlockID::Air);
				chunk->Recreate();
			}
			else if (e.GetButton() == Mouse::ButtonRight)
			{
				glm::ivec3 newBlockLocalPos = Math::GetBlockPositionInChunk(prevPosition);
				glm::ivec2 newBlockChunkPos = Math::GetChunkPositionFromWorldPosition(prevPosition);
				chunk = m_ChunkMap[newBlockChunkPos];

				chunk->SetBlock(newBlockLocalPos.x, newBlockLocalPos.y, newBlockLocalPos.z, m_SelectedBlock);
				chunk->Recreate();
			}
			break;
		}
		prevPosition = currentPosition;
	}

	return false;
}

bool GameLayer::OnKeyPressed(KeyPressedEvent& e)
{
	if (e.GetKeyCode() == Key::D1)
		m_SelectedBlock = BlockID::Grass;
	else if (e.GetKeyCode() == Key::D2)
		m_SelectedBlock = BlockID::Stone;
	else if (e.GetKeyCode() == Key::D3)
		m_SelectedBlock = BlockID::Dirt;
	else if (e.GetKeyCode() == Key::D4)
		m_SelectedBlock = BlockID::Sand;
	else if (e.GetKeyCode() == Key::D5)
		m_SelectedBlock = BlockID::Wood;
	else if (e.GetKeyCode() == Key::D6)
		m_SelectedBlock = BlockID::Plank;
	else if (e.GetKeyCode() == Key::D7)
		m_SelectedBlock = BlockID::Glass;

	return false;
}

#pragma endregion

#pragma region Private Methods

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
	std::this_thread::sleep_for(std::chrono::microseconds(100));
}

void GameLayer::InitializeTextures()
{
	// Sprite sheet
	m_SpriteSheet = CreateRef<Texture2D>("assets/textures/minecraft_spritesheet.png", TextureFilter::MipmapLinear, TextureFilter::Nearest, TextureWrap::ClampToEdge, true);

	// Crosshair
	m_CrosshairTexture = CreateRef<Texture2D>("assets/textures/crosshair.png", TextureFilter::MipmapLinear, TextureFilter::Nearest, TextureWrap::ClampToEdge, true);

	// Itembox
	m_ItemBoxTexture = CreateRef<Texture2D>("assets/textures/itembox.png", TextureFilter::MipmapLinear, TextureFilter::Nearest, TextureWrap::ClampToEdge, true);

	// Create all subtextures
	SpriteManager::AddSprite("GrassTop", CreateRef<SubTexture2D>(m_SpriteSheet, 0, 15, 64, 64));
	SpriteManager::AddSprite("GrassSide", CreateRef<SubTexture2D>(m_SpriteSheet, 3, 15, 64, 64));
	SpriteManager::AddSprite("Dirt", CreateRef<SubTexture2D>(m_SpriteSheet, 2, 15, 64, 64));
	SpriteManager::AddSprite("Stone", CreateRef<SubTexture2D>(m_SpriteSheet, 1, 15, 64, 64));
	SpriteManager::AddSprite("Bedrock", CreateRef<SubTexture2D>(m_SpriteSheet, 1, 14, 64, 64));
	SpriteManager::AddSprite("Water", CreateRef<SubTexture2D>(m_SpriteSheet, 13, 3, 64, 64));
	SpriteManager::AddSprite("Sand", CreateRef<SubTexture2D>(m_SpriteSheet, 2, 14, 64, 64));
	SpriteManager::AddSprite("WoodTop", CreateRef<SubTexture2D>(m_SpriteSheet, 5, 14, 64, 64));
	SpriteManager::AddSprite("Wood", CreateRef<SubTexture2D>(m_SpriteSheet, 4, 8, 64, 64));
	SpriteManager::AddSprite("Leaf", CreateRef<SubTexture2D>(m_SpriteSheet, 4, 12, 64, 64));
	SpriteManager::AddSprite("Plank", CreateRef<SubTexture2D>(m_SpriteSheet, 4, 15, 64, 64));
	SpriteManager::AddSprite("Glass", CreateRef<SubTexture2D>(m_SpriteSheet, 1, 12, 64, 64));
}

#pragma endregion


