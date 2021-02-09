#include "pch.h"
#include "Math.h"

#include "Game\Terrain\Chunk.h"

namespace Math
{
    glm::ivec3 GetBlockPositionInChunk(const glm::vec3& globalPosition)
    {
        return {
            (Chunk::WIDTH + ((int)globalPosition.x % Chunk::WIDTH)) % Chunk::WIDTH,
            (Chunk::HEIGHT + ((int)globalPosition.y % Chunk::HEIGHT)) % Chunk::HEIGHT,
            (Chunk::DEPTH + ((int)globalPosition.z % Chunk::DEPTH)) % Chunk::DEPTH
        };
    }

	glm::ivec3 GetBlockCoordinates(const glm::vec3& position)
	{
		return { (int)std::floor(position.x), (int)std::floor(position.y), (int)std::floor(position.z) };
	}

    glm::ivec2 GetChunkPositionFromWorldPosition(const glm::vec3& worldPosition)
    {
        int x, z;

        if (worldPosition.x >= 0 || (int)worldPosition.x % Chunk::WIDTH == 0)
            x = worldPosition.x / Chunk::WIDTH;
        else
            x = worldPosition.x / Chunk::WIDTH - 1;

        if (worldPosition.z >= 0 || (int)worldPosition.z % Chunk::DEPTH == 0)
            z = worldPosition.z / Chunk::DEPTH;
        else
            z = worldPosition.z / Chunk::DEPTH - 1;


        return { x, z };
    }

    std::vector<glm::vec3> CastRayAndGetIntersectingBlocks(const glm::vec3& origin, const glm::vec3& direction, float distance)
    {
		glm::ivec3 currentCubePosition = GetBlockCoordinates(origin);
		glm::vec3 endPoint = origin + distance * direction;

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

		while (intersected.size() < distance * 3)
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

		return intersected;
    }
}
