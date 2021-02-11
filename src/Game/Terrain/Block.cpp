#include "pch.h"
#include "Block.h"

#include "Graphics\SpriteManager.h"

std::vector<ScopedPtr<Block>> Block::s_Blocks;

Block::Block(BlockID id)
	: m_ID(id)
{
	glm::vec3 positions[8];
	positions[0] = {  0.0f,  0.0f,  1.0f };
	positions[1] = {  1.0f,  0.0f,  1.0f };
	positions[2] = {  1.0f,  1.0f,  1.0f };
	positions[3] = {  0.0f,  1.0f,  1.0f };
	positions[4] = {  0.0f,  0.0f,  0.0f };
	positions[5] = {  1.0f,  0.0f,  0.0f };
	positions[6] = {  1.0f,  1.0f,  0.0f };
	positions[7] = {  0.0f,  1.0f,  0.0f };

	glm::vec3 normals[6];
	normals[(uint8_t)BlockFaceID::Front]	= {  0.0f,  0.0f,  1.0f };
	normals[(uint8_t)BlockFaceID::Back]		= {  0.0f,  0.0f, -1.0f };
	normals[(uint8_t)BlockFaceID::Left]		= { -1.0f,  0.0f,  0.0f };
	normals[(uint8_t)BlockFaceID::Right]	= {  1.0f,  0.0f,  0.0f };
	normals[(uint8_t)BlockFaceID::Up]		= {  0.0f,  1.0f,  0.0f };
	normals[(uint8_t)BlockFaceID::Down]		= {  0.0f, -1.0f,  0.0f };
	
	// Front face
	m_Faces[(uint8_t)BlockFaceID::Front].Vertices[0].Position = positions[0];
	m_Faces[(uint8_t)BlockFaceID::Front].Vertices[1].Position = positions[1];
	m_Faces[(uint8_t)BlockFaceID::Front].Vertices[2].Position = positions[2];
	m_Faces[(uint8_t)BlockFaceID::Front].Vertices[3].Position = positions[3];

	for (int i = 0; i < 4; i++)
	{
		m_Faces[(uint8_t)BlockFaceID::Front].Vertices[i].Normal = normals[(uint8_t)BlockFaceID::Front];
		m_Faces[(uint8_t)BlockFaceID::Front].Vertices[i].LightLevel = 0.8f;
	}

	// Back face
	m_Faces[(uint8_t)BlockFaceID::Back].Vertices[0].Position = positions[4];
	m_Faces[(uint8_t)BlockFaceID::Back].Vertices[1].Position = positions[5];
	m_Faces[(uint8_t)BlockFaceID::Back].Vertices[2].Position = positions[6];
	m_Faces[(uint8_t)BlockFaceID::Back].Vertices[3].Position = positions[7];

	for (int i = 0; i < 4; i++)
	{
		m_Faces[(uint8_t)BlockFaceID::Back].Vertices[i].Normal = normals[(uint8_t)BlockFaceID::Back];
		m_Faces[(uint8_t)BlockFaceID::Back].Vertices[i].LightLevel = 0.8f;
	}

	// Left face
	m_Faces[(uint8_t)BlockFaceID::Left].Vertices[0].Position = positions[4];
	m_Faces[(uint8_t)BlockFaceID::Left].Vertices[1].Position = positions[0];
	m_Faces[(uint8_t)BlockFaceID::Left].Vertices[2].Position = positions[3];
	m_Faces[(uint8_t)BlockFaceID::Left].Vertices[3].Position = positions[7];

	for (int i = 0; i < 4; i++)
	{
		m_Faces[(uint8_t)BlockFaceID::Left].Vertices[i].Normal = normals[(uint8_t)BlockFaceID::Left];
		m_Faces[(uint8_t)BlockFaceID::Left].Vertices[i].LightLevel = 0.6f;
	}

	// Right face
	m_Faces[(uint8_t)BlockFaceID::Right].Vertices[0].Position = positions[1];
	m_Faces[(uint8_t)BlockFaceID::Right].Vertices[1].Position = positions[5];
	m_Faces[(uint8_t)BlockFaceID::Right].Vertices[2].Position = positions[6];
	m_Faces[(uint8_t)BlockFaceID::Right].Vertices[3].Position = positions[2];

	for (int i = 0; i < 4; i++)
	{
		m_Faces[(uint8_t)BlockFaceID::Right].Vertices[i].Normal = normals[(uint8_t)BlockFaceID::Right];
		m_Faces[(uint8_t)BlockFaceID::Right].Vertices[i].LightLevel = 0.6f;
	}

	// Up face
	m_Faces[(uint8_t)BlockFaceID::Up].Vertices[0].Position = positions[3];
	m_Faces[(uint8_t)BlockFaceID::Up].Vertices[1].Position = positions[2];
	m_Faces[(uint8_t)BlockFaceID::Up].Vertices[2].Position = positions[6];
	m_Faces[(uint8_t)BlockFaceID::Up].Vertices[3].Position = positions[7];

	for (int i = 0; i < 4; i++)
	{
		m_Faces[(uint8_t)BlockFaceID::Up].Vertices[i].Normal = normals[(uint8_t)BlockFaceID::Up];
		m_Faces[(uint8_t)BlockFaceID::Up].Vertices[i].LightLevel = 1.0f;
	}

	// Down face
	m_Faces[(uint8_t)BlockFaceID::Down].Vertices[0].Position = positions[0];
	m_Faces[(uint8_t)BlockFaceID::Down].Vertices[1].Position = positions[1];
	m_Faces[(uint8_t)BlockFaceID::Down].Vertices[2].Position = positions[5];
	m_Faces[(uint8_t)BlockFaceID::Down].Vertices[3].Position = positions[4];

	for (int i = 0; i < 4; i++)
	{
		m_Faces[(uint8_t)BlockFaceID::Down].Vertices[i].Normal = normals[(uint8_t)BlockFaceID::Down];
		m_Faces[(uint8_t)BlockFaceID::Down].Vertices[i].LightLevel = 0.4f;
	}

	SetBlockTextures(id);
}

void Block::SetBlockTextures(BlockID block)
{
	Ref<SubTexture2D> upFace, downFace, leftFace, rightFace, frontFace, backFace;

	switch (block)
	{
		case BlockID::Grass:
		{
			upFace		= SpriteManager::GetSprite("GrassTop");
			downFace	= SpriteManager::GetSprite("Dirt");
			leftFace	= SpriteManager::GetSprite("GrassSide");
			rightFace	= SpriteManager::GetSprite("GrassSide");
			frontFace	= SpriteManager::GetSprite("GrassSide");
			backFace	= SpriteManager::GetSprite("GrassSide");
			break;
		}
		case BlockID::Dirt:
		{
			upFace		= SpriteManager::GetSprite("Dirt");
			downFace	= SpriteManager::GetSprite("Dirt");
			leftFace	= SpriteManager::GetSprite("Dirt");
			rightFace	= SpriteManager::GetSprite("Dirt");
			frontFace	= SpriteManager::GetSprite("Dirt");
			backFace	= SpriteManager::GetSprite("Dirt");
			break;
		}
		case BlockID::Stone:
		{
			upFace		= SpriteManager::GetSprite("Stone");
			downFace	= SpriteManager::GetSprite("Stone");
			leftFace	= SpriteManager::GetSprite("Stone");
			rightFace	= SpriteManager::GetSprite("Stone");
			frontFace	= SpriteManager::GetSprite("Stone");
			backFace	= SpriteManager::GetSprite("Stone");
			break;
		}
		case BlockID::Bedrock:
		{
			upFace		= SpriteManager::GetSprite("Bedrock");
			downFace	= SpriteManager::GetSprite("Bedrock");
			leftFace	= SpriteManager::GetSprite("Bedrock");
			rightFace	= SpriteManager::GetSprite("Bedrock");
			frontFace	= SpriteManager::GetSprite("Bedrock");
			backFace	= SpriteManager::GetSprite("Bedrock");
			break;
		}
		case BlockID::Water:
		{
			upFace		= SpriteManager::GetSprite("Water");
			downFace	= SpriteManager::GetSprite("Water");
			leftFace	= SpriteManager::GetSprite("Water");
			rightFace	= SpriteManager::GetSprite("Water");
			frontFace	= SpriteManager::GetSprite("Water");
			backFace	= SpriteManager::GetSprite("Water");
			break;
		}
		case BlockID::Sand:
		{
			upFace		= SpriteManager::GetSprite("Sand");
			downFace	= SpriteManager::GetSprite("Sand");
			leftFace	= SpriteManager::GetSprite("Sand");
			rightFace	= SpriteManager::GetSprite("Sand");
			frontFace	= SpriteManager::GetSprite("Sand");
			backFace	= SpriteManager::GetSprite("Sand");
			break;
		}
		case BlockID::Wood:
		{
			upFace		= SpriteManager::GetSprite("WoodTop");
			downFace	= SpriteManager::GetSprite("WoodTop");
			leftFace	= SpriteManager::GetSprite("Wood");
			rightFace	= SpriteManager::GetSprite("Wood");
			frontFace	= SpriteManager::GetSprite("Wood");
			backFace	= SpriteManager::GetSprite("Wood");
			break;
		}
		case BlockID::Leaf:
		{
			upFace		= SpriteManager::GetSprite("Leaf");
			downFace	= SpriteManager::GetSprite("Leaf");
			leftFace	= SpriteManager::GetSprite("Leaf");
			rightFace	= SpriteManager::GetSprite("Leaf");
			frontFace	= SpriteManager::GetSprite("Leaf");
			backFace	= SpriteManager::GetSprite("Leaf");
			break;
		}
		case BlockID::Plank:
		{
			upFace		= SpriteManager::GetSprite("Plank");
			downFace	= SpriteManager::GetSprite("Plank");
			leftFace	= SpriteManager::GetSprite("Plank");
			rightFace	= SpriteManager::GetSprite("Plank");
			frontFace	= SpriteManager::GetSprite("Plank");
			backFace	= SpriteManager::GetSprite("Plank");
			break;
		}
		case BlockID::Glass:
		{
			upFace		= SpriteManager::GetSprite("Glass");
			downFace	= SpriteManager::GetSprite("Glass");
			leftFace	= SpriteManager::GetSprite("Glass");
			rightFace	= SpriteManager::GetSprite("Glass");
			frontFace	= SpriteManager::GetSprite("Glass");
			backFace	= SpriteManager::GetSprite("Glass");
			break;
		}
	}

	for (int i = 0; i < 4; i++)
	{
		m_Faces[(int8_t)BlockFaceID::Up].Vertices[i].TexCoord		= upFace->GetTextureCoords()[i];
		m_Faces[(int8_t)BlockFaceID::Down].Vertices[i].TexCoord		= downFace->GetTextureCoords()[i];
		m_Faces[(int8_t)BlockFaceID::Left].Vertices[i].TexCoord		= leftFace->GetTextureCoords()[i];
		m_Faces[(int8_t)BlockFaceID::Right].Vertices[i].TexCoord	= rightFace->GetTextureCoords()[i];
		m_Faces[(int8_t)BlockFaceID::Front].Vertices[i].TexCoord	= frontFace->GetTextureCoords()[i];
		m_Faces[(int8_t)BlockFaceID::Back].Vertices[i].TexCoord		= backFace->GetTextureCoords()[i];
	}
}

void Block::CreateBlockTemplates()
{
	s_Blocks.resize(10);

	s_Blocks[(int8_t)BlockID::Grass]		= CreateScoped<Block>(BlockID::Grass);
	s_Blocks[(int8_t)BlockID::Dirt]			= CreateScoped<Block>(BlockID::Dirt);
	s_Blocks[(int8_t)BlockID::Stone]		= CreateScoped<Block>(BlockID::Stone);
	s_Blocks[(int8_t)BlockID::Bedrock]		= CreateScoped<Block>(BlockID::Bedrock);
	s_Blocks[(int8_t)BlockID::Water]		= CreateScoped<Block>(BlockID::Water);
	s_Blocks[(int8_t)BlockID::Sand]			= CreateScoped<Block>(BlockID::Sand);
	s_Blocks[(int8_t)BlockID::Wood]			= CreateScoped<Block>(BlockID::Wood);
	s_Blocks[(int8_t)BlockID::Leaf]			= CreateScoped<Block>(BlockID::Leaf);
	s_Blocks[(int8_t)BlockID::Plank]		= CreateScoped<Block>(BlockID::Plank);
	s_Blocks[(int8_t)BlockID::Glass]		= CreateScoped<Block>(BlockID::Glass);
}
