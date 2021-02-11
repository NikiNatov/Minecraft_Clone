#include "pch.h"
#include "World.h"

#include "Game\Entity\Component\TransformComponent.h"
#include "Game\Entity\Component\ScriptComponent.h"
#include "Game\Entity\Component\CameraComponent.h"

#include "../assets/scripts/FPSCameraScript.h"
#include "../assets/scripts/PlayerScript.h"

#include "Graphics\Renderer.h"

World::World()
{
	m_Player = CreateRef<Entity>(glm::vec3(0.0f, 60.0f, 0.0), glm::vec3(0.0f), glm::vec3(1.0f), this);
	m_Player->AddComponent<ScriptComponent>()->Bind<PlayerScript>();
	m_Player->GetComponent<ScriptComponent>()->GetInstance()->OnCreate();

	glm::vec3 playerPos = m_Player->GetComponent<TransformComponent>()->GetPosition();
	glm::vec3 playerRot = m_Player->GetComponent<TransformComponent>()->GetRotation();

	m_Camera = CreateRef<Entity>(playerPos, playerRot, glm::vec3(1.0f), this);
	m_Camera->AddComponent<CameraComponent>(45.0f, 16.0f / 9.0f, 0.01f, 1000.0f);
	m_Camera->AddComponent<ScriptComponent>()->Bind<FPSCameraScript>();
	m_Camera->GetComponent<ScriptComponent>()->GetInstance()->OnCreate();
}

World::~World()
{
}

void World::OnUpdate(float dt)
{
	m_Player->GetComponent<ScriptComponent>()->GetInstance()->OnUpdate(dt);
	m_Camera->GetComponent<ScriptComponent>()->GetInstance()->OnUpdate(dt);

	m_Camera->GetComponent<TransformComponent>()->GetPosition() = m_Player->GetComponent<TransformComponent>()->GetPosition();
	m_Camera->GetComponent<TransformComponent>()->GetRotation().y = m_Player->GetComponent<TransformComponent>()->GetRotation().y;
}

void World::RenderChunks()
{
	glm::mat4 cameraTransform = m_Camera->GetComponent<TransformComponent>()->GetTransformMatrix();
	Camera& camera = m_Camera->GetComponent<CameraComponent>()->GetCamera();

	Renderer::BeginScene(camera, cameraTransform);
	Renderer::ClearScreen({ 0.4f, 0.7f, 1.0f, 1.0f });

	for (auto it = m_Chunks.begin(); it != m_Chunks.end(); it++)
	{
		Ref<Chunk> chunk(it->second);

		if (!chunk->GetVAO())
			chunk->InitializeVAO();

		Renderer::Submit(chunk->GetVAO(), glm::translate(glm::mat4(1.0f), glm::vec3(chunk->GetGridPosition().x * 16.0f, 0.0f, chunk->GetGridPosition().z * 16.0f)));
	}

	Renderer::EndScene();
	Renderer::Present();
	
}

void World::UpdateChunks()
{
	glm::vec3 playerPos = m_Player->GetComponent<TransformComponent>()->GetPosition();

	int currentChunkX = playerPos.x / Chunk::WIDTH;
	int currentChunkZ = playerPos.z / Chunk::DEPTH;

	for (auto it = m_Chunks.begin(); it != m_Chunks.end();)
	{
		if (it->first.x > currentChunkX + m_ChunkRenderDistance || it->first.x < currentChunkX - m_ChunkRenderDistance ||
			it->first.y > currentChunkZ + m_ChunkRenderDistance || it->first.y < currentChunkZ - m_ChunkRenderDistance)
		{
			std::lock_guard<std::mutex> lock(m_MainMutex);
			it = m_Chunks.erase(it);
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
				if (m_Chunks.find({ currentChunkX + x, currentChunkZ + z }) == m_Chunks.end())
				{
					std::lock_guard<std::mutex> lock(m_MainMutex);
					m_Chunks[{ currentChunkX + x, currentChunkZ + z }] = CreateRef<Chunk>(glm::vec3(currentChunkX + x, 0, currentChunkZ + z));
				}
			}
		}
	}
}

void World::SetBlock(const glm::vec3& position, BlockID block)
{
	glm::ivec3 blockPosInChunk = Math::GetBlockPositionInChunk(position);
	GetChunk(position)->SetBlock(blockPosInChunk.x, blockPosInChunk.y, blockPosInChunk.z, block);
}

Ref<Entity> World::GetCameraEntity() const
{
	return m_Camera;
}

Ref<Entity> World::GetPlayerEntity() const
{
	return m_Player;
}

BlockID World::GetBlock(const glm::vec3& position) const
{
	glm::ivec3 blockPosInChunk = Math::GetBlockPositionInChunk(position);
	return GetChunk(position)->GetBlock(blockPosInChunk.x, blockPosInChunk.y, blockPosInChunk.z);
}

Ref<Chunk> World::GetChunk(const glm::vec3& position) const
{
	glm::ivec2 chunkPos = Math::GetChunkPositionFromWorldPosition(position);
	ASSERT(m_Chunks.find(chunkPos) != m_Chunks.end(), "Chunk does not exist!");
	return m_Chunks.at(chunkPos);
}
