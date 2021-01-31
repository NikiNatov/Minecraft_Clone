#include "pch.h"
#include "Block.h"

#include "Graphics\SpriteManager.h"

ScopedPtr<Block> Block::s_GrassBlock;
ScopedPtr<Block> Block::s_DirtBlock ;
ScopedPtr<Block> Block::s_StoneBlock;

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
	normals[(uint8_t)BlockFaceID::Front]	= { 0.0f,  0.0f,  1.0f };
	normals[(uint8_t)BlockFaceID::Back]		= { 0.0f,  0.0f, -1.0f };
	normals[(uint8_t)BlockFaceID::Left]		= { -1.0f, 0.0f,  0.0f };
	normals[(uint8_t)BlockFaceID::Right]	= { 1.0f,  0.0f,  0.0f };
	normals[(uint8_t)BlockFaceID::Up]		= { 0.0f,  1.0f,  0.0f };
	normals[(uint8_t)BlockFaceID::Down]		= { 0.0f, -1.0f,  0.0f };
	
	// Front face
	m_Faces[(uint8_t)BlockFaceID::Front].Vertices[0].Position = positions[0];
	m_Faces[(uint8_t)BlockFaceID::Front].Vertices[1].Position = positions[1];
	m_Faces[(uint8_t)BlockFaceID::Front].Vertices[2].Position = positions[2];
	m_Faces[(uint8_t)BlockFaceID::Front].Vertices[3].Position = positions[3];

	m_Faces[(uint8_t)BlockFaceID::Front].Vertices[0].Normal = normals[(uint8_t)BlockFaceID::Front];
	m_Faces[(uint8_t)BlockFaceID::Front].Vertices[1].Normal = normals[(uint8_t)BlockFaceID::Front];
	m_Faces[(uint8_t)BlockFaceID::Front].Vertices[2].Normal = normals[(uint8_t)BlockFaceID::Front];
	m_Faces[(uint8_t)BlockFaceID::Front].Vertices[3].Normal = normals[(uint8_t)BlockFaceID::Front];

	// Back face
	m_Faces[(uint8_t)BlockFaceID::Back].Vertices[0].Position = positions[4];
	m_Faces[(uint8_t)BlockFaceID::Back].Vertices[1].Position = positions[5];
	m_Faces[(uint8_t)BlockFaceID::Back].Vertices[2].Position = positions[6];
	m_Faces[(uint8_t)BlockFaceID::Back].Vertices[3].Position = positions[7];

	m_Faces[(uint8_t)BlockFaceID::Back].Vertices[0].Normal = normals[(uint8_t)BlockFaceID::Back];
	m_Faces[(uint8_t)BlockFaceID::Back].Vertices[1].Normal = normals[(uint8_t)BlockFaceID::Back];
	m_Faces[(uint8_t)BlockFaceID::Back].Vertices[2].Normal = normals[(uint8_t)BlockFaceID::Back];
	m_Faces[(uint8_t)BlockFaceID::Back].Vertices[3].Normal = normals[(uint8_t)BlockFaceID::Back];

	// Left face
	m_Faces[(uint8_t)BlockFaceID::Left].Vertices[0].Position = positions[4];
	m_Faces[(uint8_t)BlockFaceID::Left].Vertices[1].Position = positions[0];
	m_Faces[(uint8_t)BlockFaceID::Left].Vertices[2].Position = positions[3];
	m_Faces[(uint8_t)BlockFaceID::Left].Vertices[3].Position = positions[7];

	m_Faces[(uint8_t)BlockFaceID::Left].Vertices[0].Normal = normals[(uint8_t)BlockFaceID::Left];
	m_Faces[(uint8_t)BlockFaceID::Left].Vertices[1].Normal = normals[(uint8_t)BlockFaceID::Left];
	m_Faces[(uint8_t)BlockFaceID::Left].Vertices[2].Normal = normals[(uint8_t)BlockFaceID::Left];
	m_Faces[(uint8_t)BlockFaceID::Left].Vertices[3].Normal = normals[(uint8_t)BlockFaceID::Left];

	// Right face
	m_Faces[(uint8_t)BlockFaceID::Right].Vertices[0].Position = positions[1];
	m_Faces[(uint8_t)BlockFaceID::Right].Vertices[1].Position = positions[5];
	m_Faces[(uint8_t)BlockFaceID::Right].Vertices[2].Position = positions[6];
	m_Faces[(uint8_t)BlockFaceID::Right].Vertices[3].Position = positions[2];

	m_Faces[(uint8_t)BlockFaceID::Right].Vertices[0].Normal = normals[(uint8_t)BlockFaceID::Right];
	m_Faces[(uint8_t)BlockFaceID::Right].Vertices[1].Normal = normals[(uint8_t)BlockFaceID::Right];
	m_Faces[(uint8_t)BlockFaceID::Right].Vertices[2].Normal = normals[(uint8_t)BlockFaceID::Right];
	m_Faces[(uint8_t)BlockFaceID::Right].Vertices[3].Normal = normals[(uint8_t)BlockFaceID::Right];

	// Up face
	m_Faces[(uint8_t)BlockFaceID::Up].Vertices[0].Position = positions[3];
	m_Faces[(uint8_t)BlockFaceID::Up].Vertices[1].Position = positions[2];
	m_Faces[(uint8_t)BlockFaceID::Up].Vertices[2].Position = positions[6];
	m_Faces[(uint8_t)BlockFaceID::Up].Vertices[3].Position = positions[7];

	m_Faces[(uint8_t)BlockFaceID::Up].Vertices[0].Normal = normals[(uint8_t)BlockFaceID::Up];
	m_Faces[(uint8_t)BlockFaceID::Up].Vertices[1].Normal = normals[(uint8_t)BlockFaceID::Up];
	m_Faces[(uint8_t)BlockFaceID::Up].Vertices[2].Normal = normals[(uint8_t)BlockFaceID::Up];
	m_Faces[(uint8_t)BlockFaceID::Up].Vertices[3].Normal = normals[(uint8_t)BlockFaceID::Up];

	// Down face
	m_Faces[(uint8_t)BlockFaceID::Down].Vertices[0].Position = positions[0];
	m_Faces[(uint8_t)BlockFaceID::Down].Vertices[1].Position = positions[1];
	m_Faces[(uint8_t)BlockFaceID::Down].Vertices[2].Position = positions[5];
	m_Faces[(uint8_t)BlockFaceID::Down].Vertices[3].Position = positions[4];

	m_Faces[(uint8_t)BlockFaceID::Down].Vertices[0].Normal = normals[(uint8_t)BlockFaceID::Down];
	m_Faces[(uint8_t)BlockFaceID::Down].Vertices[1].Normal = normals[(uint8_t)BlockFaceID::Down];
	m_Faces[(uint8_t)BlockFaceID::Down].Vertices[2].Normal = normals[(uint8_t)BlockFaceID::Down];
	m_Faces[(uint8_t)BlockFaceID::Down].Vertices[3].Normal = normals[(uint8_t)BlockFaceID::Down];

	switch (id)
	{
		case BlockID::Grass:
		{
			const Ref<SubTexture2D>& grassTop = SpriteManager::GetSprite("GrassTop");
			const Ref<SubTexture2D>& grassSide = SpriteManager::GetSprite("GrassSide");
			const Ref<SubTexture2D>& dirt = SpriteManager::GetSprite("Dirt");

			// Up face
			m_Faces[(uint8_t)BlockFaceID::Up].Vertices[0].TexCoord = grassTop->GetTextureCoords()[0];
			m_Faces[(uint8_t)BlockFaceID::Up].Vertices[1].TexCoord = grassTop->GetTextureCoords()[1];
			m_Faces[(uint8_t)BlockFaceID::Up].Vertices[2].TexCoord = grassTop->GetTextureCoords()[2];
			m_Faces[(uint8_t)BlockFaceID::Up].Vertices[3].TexCoord = grassTop->GetTextureCoords()[3];

			// Down face
			m_Faces[(uint8_t)BlockFaceID::Down].Vertices[0].TexCoord = dirt->GetTextureCoords()[0];
			m_Faces[(uint8_t)BlockFaceID::Down].Vertices[1].TexCoord = dirt->GetTextureCoords()[1];
			m_Faces[(uint8_t)BlockFaceID::Down].Vertices[2].TexCoord = dirt->GetTextureCoords()[2];
			m_Faces[(uint8_t)BlockFaceID::Down].Vertices[3].TexCoord = dirt->GetTextureCoords()[3];

			// Front face
			m_Faces[(uint8_t)BlockFaceID::Front].Vertices[0].TexCoord = grassSide->GetTextureCoords()[0];
			m_Faces[(uint8_t)BlockFaceID::Front].Vertices[1].TexCoord = grassSide->GetTextureCoords()[1];
			m_Faces[(uint8_t)BlockFaceID::Front].Vertices[2].TexCoord = grassSide->GetTextureCoords()[2];
			m_Faces[(uint8_t)BlockFaceID::Front].Vertices[3].TexCoord = grassSide->GetTextureCoords()[3];

			// Back face
			m_Faces[(uint8_t)BlockFaceID::Back].Vertices[0].TexCoord = grassSide->GetTextureCoords()[0];
			m_Faces[(uint8_t)BlockFaceID::Back].Vertices[1].TexCoord = grassSide->GetTextureCoords()[1];
			m_Faces[(uint8_t)BlockFaceID::Back].Vertices[2].TexCoord = grassSide->GetTextureCoords()[2];
			m_Faces[(uint8_t)BlockFaceID::Back].Vertices[3].TexCoord = grassSide->GetTextureCoords()[3];

			// Left face
			m_Faces[(uint8_t)BlockFaceID::Left].Vertices[0].TexCoord = grassSide->GetTextureCoords()[0];
			m_Faces[(uint8_t)BlockFaceID::Left].Vertices[1].TexCoord = grassSide->GetTextureCoords()[1];
			m_Faces[(uint8_t)BlockFaceID::Left].Vertices[2].TexCoord = grassSide->GetTextureCoords()[2];
			m_Faces[(uint8_t)BlockFaceID::Left].Vertices[3].TexCoord = grassSide->GetTextureCoords()[3];

			// Right face
			m_Faces[(uint8_t)BlockFaceID::Right].Vertices[0].TexCoord = grassSide->GetTextureCoords()[0];
			m_Faces[(uint8_t)BlockFaceID::Right].Vertices[1].TexCoord = grassSide->GetTextureCoords()[1];
			m_Faces[(uint8_t)BlockFaceID::Right].Vertices[2].TexCoord = grassSide->GetTextureCoords()[2];
			m_Faces[(uint8_t)BlockFaceID::Right].Vertices[3].TexCoord = grassSide->GetTextureCoords()[3];

			break;
		}
		case BlockID::Dirt:
		{
			const Ref<SubTexture2D>& dirt = SpriteManager::GetSprite("Dirt");

			// Up face
			m_Faces[(uint8_t)BlockFaceID::Up].Vertices[0].TexCoord = dirt->GetTextureCoords()[0];
			m_Faces[(uint8_t)BlockFaceID::Up].Vertices[1].TexCoord = dirt->GetTextureCoords()[1];
			m_Faces[(uint8_t)BlockFaceID::Up].Vertices[2].TexCoord = dirt->GetTextureCoords()[2];
			m_Faces[(uint8_t)BlockFaceID::Up].Vertices[3].TexCoord = dirt->GetTextureCoords()[3];

			// Down face
			m_Faces[(uint8_t)BlockFaceID::Down].Vertices[0].TexCoord = dirt->GetTextureCoords()[0];
			m_Faces[(uint8_t)BlockFaceID::Down].Vertices[1].TexCoord = dirt->GetTextureCoords()[1];
			m_Faces[(uint8_t)BlockFaceID::Down].Vertices[2].TexCoord = dirt->GetTextureCoords()[2];
			m_Faces[(uint8_t)BlockFaceID::Down].Vertices[3].TexCoord = dirt->GetTextureCoords()[3];

			// Front face
			m_Faces[(uint8_t)BlockFaceID::Front].Vertices[0].TexCoord = dirt->GetTextureCoords()[0];
			m_Faces[(uint8_t)BlockFaceID::Front].Vertices[1].TexCoord = dirt->GetTextureCoords()[1];
			m_Faces[(uint8_t)BlockFaceID::Front].Vertices[2].TexCoord = dirt->GetTextureCoords()[2];
			m_Faces[(uint8_t)BlockFaceID::Front].Vertices[3].TexCoord = dirt->GetTextureCoords()[3];

			// Back face
			m_Faces[(uint8_t)BlockFaceID::Back].Vertices[0].TexCoord = dirt->GetTextureCoords()[0];
			m_Faces[(uint8_t)BlockFaceID::Back].Vertices[1].TexCoord = dirt->GetTextureCoords()[1];
			m_Faces[(uint8_t)BlockFaceID::Back].Vertices[2].TexCoord = dirt->GetTextureCoords()[2];
			m_Faces[(uint8_t)BlockFaceID::Back].Vertices[3].TexCoord = dirt->GetTextureCoords()[3];

			// Left face
			m_Faces[(uint8_t)BlockFaceID::Left].Vertices[0].TexCoord = dirt->GetTextureCoords()[0];
			m_Faces[(uint8_t)BlockFaceID::Left].Vertices[1].TexCoord = dirt->GetTextureCoords()[1];
			m_Faces[(uint8_t)BlockFaceID::Left].Vertices[2].TexCoord = dirt->GetTextureCoords()[2];
			m_Faces[(uint8_t)BlockFaceID::Left].Vertices[3].TexCoord = dirt->GetTextureCoords()[3];

			// Right face
			m_Faces[(uint8_t)BlockFaceID::Right].Vertices[0].TexCoord = dirt->GetTextureCoords()[0];
			m_Faces[(uint8_t)BlockFaceID::Right].Vertices[1].TexCoord = dirt->GetTextureCoords()[1];
			m_Faces[(uint8_t)BlockFaceID::Right].Vertices[2].TexCoord = dirt->GetTextureCoords()[2];
			m_Faces[(uint8_t)BlockFaceID::Right].Vertices[3].TexCoord = dirt->GetTextureCoords()[3];
			break;
		}
		case BlockID::Stone:
		{
			const Ref<SubTexture2D>& stone = SpriteManager::GetSprite("Stone");

			// Up face
			m_Faces[(uint8_t)BlockFaceID::Up].Vertices[0].TexCoord = stone->GetTextureCoords()[0];
			m_Faces[(uint8_t)BlockFaceID::Up].Vertices[1].TexCoord = stone->GetTextureCoords()[1];
			m_Faces[(uint8_t)BlockFaceID::Up].Vertices[2].TexCoord = stone->GetTextureCoords()[2];
			m_Faces[(uint8_t)BlockFaceID::Up].Vertices[3].TexCoord = stone->GetTextureCoords()[3];

			// Down face
			m_Faces[(uint8_t)BlockFaceID::Down].Vertices[0].TexCoord =  stone->GetTextureCoords()[0];
			m_Faces[(uint8_t)BlockFaceID::Down].Vertices[1].TexCoord =  stone->GetTextureCoords()[1];
			m_Faces[(uint8_t)BlockFaceID::Down].Vertices[2].TexCoord =  stone->GetTextureCoords()[2];
			m_Faces[(uint8_t)BlockFaceID::Down].Vertices[3].TexCoord =  stone->GetTextureCoords()[3];

			// Front face
			m_Faces[(uint8_t)BlockFaceID::Front].Vertices[0].TexCoord = stone->GetTextureCoords()[0];
			m_Faces[(uint8_t)BlockFaceID::Front].Vertices[1].TexCoord = stone->GetTextureCoords()[1];
			m_Faces[(uint8_t)BlockFaceID::Front].Vertices[2].TexCoord = stone->GetTextureCoords()[2];
			m_Faces[(uint8_t)BlockFaceID::Front].Vertices[3].TexCoord = stone->GetTextureCoords()[3];

			// Back face
			m_Faces[(uint8_t)BlockFaceID::Back].Vertices[0].TexCoord = stone->GetTextureCoords()[0];
			m_Faces[(uint8_t)BlockFaceID::Back].Vertices[1].TexCoord = stone->GetTextureCoords()[1];
			m_Faces[(uint8_t)BlockFaceID::Back].Vertices[2].TexCoord = stone->GetTextureCoords()[2];
			m_Faces[(uint8_t)BlockFaceID::Back].Vertices[3].TexCoord = stone->GetTextureCoords()[3];

			// Left face
			m_Faces[(uint8_t)BlockFaceID::Left].Vertices[0].TexCoord = stone->GetTextureCoords()[0];
			m_Faces[(uint8_t)BlockFaceID::Left].Vertices[1].TexCoord = stone->GetTextureCoords()[1];
			m_Faces[(uint8_t)BlockFaceID::Left].Vertices[2].TexCoord = stone->GetTextureCoords()[2];
			m_Faces[(uint8_t)BlockFaceID::Left].Vertices[3].TexCoord = stone->GetTextureCoords()[3];

			// Right face
			m_Faces[(uint8_t)BlockFaceID::Right].Vertices[0].TexCoord = stone->GetTextureCoords()[0];
			m_Faces[(uint8_t)BlockFaceID::Right].Vertices[1].TexCoord = stone->GetTextureCoords()[1];
			m_Faces[(uint8_t)BlockFaceID::Right].Vertices[2].TexCoord = stone->GetTextureCoords()[2];
			m_Faces[(uint8_t)BlockFaceID::Right].Vertices[3].TexCoord = stone->GetTextureCoords()[3];

			break;
		}
	}
	// Up face
	m_Faces[(uint8_t)BlockFaceID::Up].Indices[0] = 3;
	m_Faces[(uint8_t)BlockFaceID::Up].Indices[1] = 2;
	m_Faces[(uint8_t)BlockFaceID::Up].Indices[2] = 6;
	m_Faces[(uint8_t)BlockFaceID::Up].Indices[3] = 6;
	m_Faces[(uint8_t)BlockFaceID::Up].Indices[4] = 7;
	m_Faces[(uint8_t)BlockFaceID::Up].Indices[5] = 3;

	// Down face
	m_Faces[(uint8_t)BlockFaceID::Down].Indices[0] = 0;
	m_Faces[(uint8_t)BlockFaceID::Down].Indices[1] = 1;
	m_Faces[(uint8_t)BlockFaceID::Down].Indices[2] = 5;
	m_Faces[(uint8_t)BlockFaceID::Down].Indices[3] = 5;
	m_Faces[(uint8_t)BlockFaceID::Down].Indices[4] = 4;
	m_Faces[(uint8_t)BlockFaceID::Down].Indices[5] = 0;

	// Front face
	m_Faces[(uint8_t)BlockFaceID::Front].Indices[0] = 0;
	m_Faces[(uint8_t)BlockFaceID::Front].Indices[1] = 1;
	m_Faces[(uint8_t)BlockFaceID::Front].Indices[2] = 2;
	m_Faces[(uint8_t)BlockFaceID::Front].Indices[3] = 2;
	m_Faces[(uint8_t)BlockFaceID::Front].Indices[4] = 3;
	m_Faces[(uint8_t)BlockFaceID::Front].Indices[5] = 0;

	// Back face
	m_Faces[(uint8_t)BlockFaceID::Back].Indices[0] = 4;
	m_Faces[(uint8_t)BlockFaceID::Back].Indices[1] = 5;
	m_Faces[(uint8_t)BlockFaceID::Back].Indices[2] = 6;
	m_Faces[(uint8_t)BlockFaceID::Back].Indices[3] = 6;
	m_Faces[(uint8_t)BlockFaceID::Back].Indices[4] = 7;
	m_Faces[(uint8_t)BlockFaceID::Back].Indices[5] = 4;

	// Left face
	m_Faces[(uint8_t)BlockFaceID::Left].Indices[0] = 0;
	m_Faces[(uint8_t)BlockFaceID::Left].Indices[1] = 4;
	m_Faces[(uint8_t)BlockFaceID::Left].Indices[2] = 7;
	m_Faces[(uint8_t)BlockFaceID::Left].Indices[3] = 7;
	m_Faces[(uint8_t)BlockFaceID::Left].Indices[4] = 3;
	m_Faces[(uint8_t)BlockFaceID::Left].Indices[5] = 0;

	// Right face
	m_Faces[(uint8_t)BlockFaceID::Right].Indices[0] = 1;
	m_Faces[(uint8_t)BlockFaceID::Right].Indices[1] = 5;
	m_Faces[(uint8_t)BlockFaceID::Right].Indices[2] = 6;
	m_Faces[(uint8_t)BlockFaceID::Right].Indices[3] = 6;
	m_Faces[(uint8_t)BlockFaceID::Right].Indices[4] = 2;
	m_Faces[(uint8_t)BlockFaceID::Right].Indices[5] = 1;
	
}
