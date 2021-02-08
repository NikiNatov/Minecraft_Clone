#pragma once

#include <glm\glm.hpp>

namespace Math
{
	struct Vec2Hash
	{
		std::size_t operator()(const glm::vec2& vec) const
		{
			return std::hash<float>()(vec.x) ^ std::hash<float>()(vec.y);
		}
	};

	struct Vec3Hash
	{
		std::size_t operator()(const glm::vec3& vec) const
		{
			return std::hash<float>()(vec.x) ^ std::hash<float>()(vec.y) ^ std::hash<float>()(vec.z);
		}
	};

	glm::ivec3 GetBlockPositionInChunk(const glm::vec3& globalPosition);
	glm::ivec2 GetChunkPositionFromWorldPosition(const glm::vec3& worldPosition);
	std::vector<glm::vec3> CastRayAndGetIntersectingBlocks(const glm::vec3& origin, const glm::vec3& direction, float distance);
}