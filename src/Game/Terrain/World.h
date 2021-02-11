#pragma once

#include <future>

#include "Game\Terrain\Chunk.h"
#include "Game\Entity\Entity.h"

#include "Game\Utils\Math.h"

class World
{
	using ChunkMap = std::unordered_map<glm::ivec2, Ref<Chunk>, Math::IVec2Hash>;
public:
	World();
	~World();

	void OnUpdate(float dt);
	void RenderChunks();

	void UpdateChunks();

	void SetBlock(const glm::vec3& position, BlockID block);

	BlockID GetBlock(const glm::vec3& position) const;
	Ref<Chunk> GetChunk(const glm::vec3& position) const;
	Ref<Entity> GetCameraEntity() const;
	Ref<Entity> GetPlayerEntity() const;
private:
	ChunkMap m_Chunks;

	Ref<Entity> m_Player;
	Ref<Entity> m_Camera;

	std::mutex m_MainMutex;
	float m_ChunkRenderDistance = 8.0f;
};