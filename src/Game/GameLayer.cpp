#include "pch.h"
#include "GameLayer.h"

#include "Core\Base.h"
#include "Graphics\Renderer.h"
#include "Graphics\Renderer2D.h"
#include "Graphics\SpriteManager.h"

#include "Core\Input.h"
#include "Core\MouseCodes.h"
#include "Core\KeyCodes.h"

GameLayer::GameLayer()
	: m_Camera({0.0f, 60.0f, 0.0}, 45.0f, 16.0f / 9.0f)
{
}

GameLayer::~GameLayer()
{
	m_Running = false;
	m_ChunkThread.join();
}

void GameLayer::OnAttach()
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

	if (Input::IsMouseButtonPressed(Mouse::ButtonLeft))
	{
		

	}
	if (Input::IsMouseButtonPressed(Mouse::ButtonRight))
	{

	}

	// Render Chunks
	Renderer::BeginScene(m_Camera);
	Renderer::ClearScreen({ 0.4f, 0.7f, 1.0f, 1.0f });

	m_SpriteSheet->Bind(0);

	for (auto [position, chunk] : m_ChunkMap)
	{
		if (chunk && !chunk->GetVAO())
			chunk->InitializeVAO();

		Renderer::Submit(chunk->GetVAO(), glm::translate(glm::mat4(1.0f), glm::vec3(chunk->GetGridPosition().x * 16.0f, 0.0f, chunk->GetGridPosition().z * 16.0f)));
	}

	Renderer::EndScene();
	Renderer::Present();

	// Render UI
	// TODO: move into separate layer
	Renderer2D::BeginScene(glm::ortho(-16.0f, 16.0f, 9.0f, -9.0f, 0.0f, 1.0f));
	Renderer2D::DrawQuad(glm::mat4(1.0f), m_CrosshairTexture);
	Renderer2D::DrawQuad(glm::translate(glm::mat4(1.0f), glm::vec3(-4.0f, 7.5f, 0.0f)), m_ItemBoxTexture);
	Renderer2D::DrawQuad(glm::translate(glm::mat4(1.0f), glm::vec3(-2.66f, 7.5f, 0.0f)), m_ItemBoxTexture);
	Renderer2D::DrawQuad(glm::translate(glm::mat4(1.0f), glm::vec3(-1.33f, 7.5f, 0.0f)), m_ItemBoxTexture);
	Renderer2D::DrawQuad(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 7.5f, 0.0f)), m_ItemBoxTexture);
	Renderer2D::DrawQuad(glm::translate(glm::mat4(1.0f), glm::vec3(1.33f, 7.5f, 0.0f)), m_ItemBoxTexture);
	Renderer2D::DrawQuad(glm::translate(glm::mat4(1.0f), glm::vec3(2.66f, 7.5f, 0.0f)), m_ItemBoxTexture);
	Renderer2D::DrawQuad(glm::translate(glm::mat4(1.0f), glm::vec3(4.0f, 7.5f, 0.0f)), m_ItemBoxTexture);
}

void GameLayer::OnEvent(Event& e)
{
	EventDispatcher dispatcher(e);
	dispatcher.Dispatch<WindowResizedEvent>(BIND_FN(GameLayer::OnWindowResized));
	dispatcher.Dispatch<MouseButtonPressedEvent>(BIND_FN(GameLayer::OnMouseButtonClicked));
}

bool GameLayer::OnWindowResized(WindowResizedEvent& e)
{
	m_Camera.SetProjection(45.0f, (float)e.GetWidth() / (float)e.GetHeight());
	return false;
}

bool GameLayer::OnMouseButtonClicked(MouseButtonPressedEvent& e)
{

	glm::vec3 origin = m_Camera.GetPosition();

	glm::ivec3 currentCubePosition = { (int)std::floor(origin.x), (int)std::floor(origin.y), (int)std::floor(origin.z) };
	glm::vec3 direction = glm::normalize(m_Camera.GetCameraFront());

	glm::vec3 endPoint = origin + m_BreakDistance * direction;

	int stepX = direction.x > 0.0f ? 1 : (direction.x < 0.0f ? -1 : 0);
	int stepY = direction.y > 0.0f ? 1 : (direction.y < 0.0f ? -1 : 0);
	int stepZ = direction.z > 0.0f ? 1 : (direction.z < 0.0f ? -1 : 0);

	glm::vec3 delta = { (stepX != 0) ? fmin(stepX / (endPoint.x - origin.x), FLT_MAX) : FLT_MAX,
						(stepY != 0) ? fmin(stepY / (endPoint.y - origin.y), FLT_MAX) : FLT_MAX,
						(stepZ != 0) ? fmin(stepZ / (endPoint.z - origin.z), FLT_MAX) : FLT_MAX };

	float tMaxX = (stepX > 0.0f) ? delta.x * (1.0f - origin.x + currentCubePosition.x) : delta.x * (origin.x - currentCubePosition.x);
	float tMaxY = (stepY > 0.0f) ? delta.y * (1.0f - origin.y + currentCubePosition.y) : delta.y * (origin.y - currentCubePosition.y);
	float tMaxZ = (stepZ > 0.0f) ? delta.z * (1.0f - origin.z + currentCubePosition.z) : delta.z * (origin.z - currentCubePosition.z);

	std::vector<glm::vec3> intersected;

	while (intersected.size() < m_BreakDistance * 3)
	{
		if (tMaxX < tMaxY)
		{
			if (tMaxX < tMaxZ)
			{
				tMaxX += delta.x;
				currentCubePosition.x += stepX;
			}
			else
			{
				tMaxZ += delta.z;
				currentCubePosition.z += stepZ;
			}
		}
		else
		{
			if (tMaxY < tMaxZ)
			{
				tMaxY += delta.y;
				currentCubePosition.y += stepY;
			}
			else
			{
				tMaxZ += delta.z;
				currentCubePosition.z += stepZ;
			}
		}
		if (tMaxX > 1 && tMaxY > 1 && tMaxZ > 1)
			break;
		intersected.push_back(currentCubePosition);
	}

	glm::vec3 prevPosition = intersected[0];

	for (int i = 0; i < intersected.size(); i++)
	{
		glm::ivec3 localPos = {
			(Chunk::WIDTH + ((int)intersected[i].x % Chunk::WIDTH)) % Chunk::WIDTH,
			(Chunk::HEIGHT + ((int)intersected[i].y % Chunk::HEIGHT)) % Chunk::HEIGHT,
			(Chunk::DEPTH + ((int)intersected[i].z % Chunk::DEPTH)) % Chunk::DEPTH
		};
		glm::ivec2 chunkPos = { intersected[i].x >= 0 ? intersected[i].x / Chunk::WIDTH : ((int)intersected[i].x % Chunk::WIDTH == 0) ? intersected[i].x / Chunk::WIDTH : intersected[i].x / Chunk::WIDTH - 1,
								intersected[i].z >= 0 ? intersected[i].z / Chunk::DEPTH : ((int)intersected[i].z % Chunk::DEPTH == 0) ? intersected[i].z / Chunk::DEPTH : intersected[i].z / Chunk::DEPTH - 1 };

		Ref<Chunk> chunk = m_ChunkMap[chunkPos];
		BlockID block = (chunk->GetBlock(localPos.x, localPos.y, localPos.z));

		if (block != BlockID::Air)
		{
			if (e.GetButton() == Mouse::ButtonLeft)
			{
				std::cout << (int)block << std::endl;
				chunk->SetBlock(localPos.x, localPos.y, localPos.z, BlockID::Air);
				chunk->CreateMeshData();
				chunk->InitializeVAO();
			}
			else if (e.GetButton() == Mouse::ButtonRight)
			{
				localPos = {
					(Chunk::WIDTH + ((int)prevPosition.x % Chunk::WIDTH)) % Chunk::WIDTH,
					(Chunk::HEIGHT + ((int)prevPosition.y % Chunk::HEIGHT)) % Chunk::HEIGHT,
					(Chunk::DEPTH + ((int)prevPosition.z % Chunk::DEPTH)) % Chunk::DEPTH
				};
				chunkPos = { prevPosition.x >= 0 ? prevPosition.x / Chunk::WIDTH : ((int)prevPosition.x % Chunk::WIDTH == 0) ? prevPosition.x / Chunk::WIDTH : prevPosition.x / Chunk::WIDTH - 1,
										prevPosition.z >= 0 ? prevPosition.z / Chunk::DEPTH : ((int)prevPosition.z % Chunk::DEPTH == 0) ? prevPosition.z / Chunk::DEPTH : prevPosition.z / Chunk::DEPTH - 1 };

				chunk = m_ChunkMap[chunkPos];
				chunk->SetBlock(localPos.x, localPos.y, localPos.z, BlockID::Stone);
			}
			
			chunk->CreateMeshData();
			chunk->InitializeVAO();

			break;
		}
		prevPosition = intersected[i];
	}

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
				std::unique_lock<std::mutex> lock(m_Mutex);
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
						std::unique_lock<std::mutex> lock(m_Mutex);
						m_ChunkMap[{ currentChunkX + x, currentChunkZ + z }] = CreateRef<Chunk>(glm::vec3(currentChunkX + x, 0, currentChunkZ + z));
					}
				}
			}
		}
	}
}