#include "pch.h"
#include "GameLayer.h"

#include "Core\Base.h"
#include "Graphics\Renderer.h"
#include "Graphics\Renderer2D.h"
#include "Graphics\SpriteManager.h"

#include "Core\Input.h"
#include "Core\MouseCodes.h"
#include "Core\KeyCodes.h"

#include "Game\Entity\Component\CameraComponent.h"
#include "Game\Entity\Component\TransformComponent.h"
#include "Game\Entity\Component\ScriptComponent.h"

#include "../assets/scripts/FPSCameraScript.h"
#include "../assets/scripts/PlayerScript.h"

#pragma region Constructors / Destructors

GameLayer::GameLayer()
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

	m_World = CreateRef<World>();
	// Chunk update thread
	m_ChunkThread = std::thread([&]()
	{
		while (m_Running)
		{
			m_World->UpdateChunks();
			std::this_thread::sleep_for(std::chrono::microseconds(100));
		}

	});
}

void GameLayer::OnDetach()
{
}

void GameLayer::OnUpdate(float dt)
{
	m_World->OnUpdate(dt);

	// Render Chunks

	m_SpriteSheet->Bind(0);
	m_World->RenderChunks();

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
	Camera& camera = m_World->GetCameraEntity()->GetComponent<CameraComponent>()->GetCamera();
	camera.SetProjection(45.0f, (float)e.GetWidth() / (float)e.GetHeight(), 0.01f, 1000.0f);
	return false;
}

bool GameLayer::OnMouseButtonClicked(MouseButtonPressedEvent& e)
{
	glm::vec3 playerPos = m_World->GetPlayerEntity()->GetComponent<TransformComponent>()->GetPosition();
	glm::vec3 direction = m_World->GetCameraEntity()->GetComponent<TransformComponent>()->GetFront();
	auto intersected = Math::CastRayAndGetIntersectingBlocks(playerPos, direction, m_BreakDistance);

	glm::vec3 prevPosition = intersected.front();

	for (const auto& currentPosition : intersected)
	{
		BlockID block = m_World->GetBlock(currentPosition);

		if (block != BlockID::Air && block != BlockID::Water)
		{
			if (e.GetButton() == Mouse::ButtonLeft)
			{
				m_World->SetBlock(currentPosition, BlockID::Air);
				m_World->GetChunk(currentPosition)->Recreate();
			}
			else if (e.GetButton() == Mouse::ButtonRight)
			{
				m_World->SetBlock(prevPosition, m_SelectedBlock);
				m_World->GetChunk(prevPosition)->Recreate();
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


