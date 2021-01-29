#include "pch.h"
#include "GameLayer.h"

#include "Core\Base.h"


GameLayer::GameLayer()
{
}

GameLayer::~GameLayer()
{
}

void GameLayer::OnAttach()
{
	std::cout << "Game Layer: Layer attached" << std::endl;

	BufferLayout layout = {
		{"a_Position", DataType::Float3, false}
	};

	float vertices[] =
	{
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
	};

	Ref<VertexBuffer> vb = CreateRef<VertexBuffer>(vertices, sizeof(vertices));
	vb->SetLayout(layout);

	uint32_t indices[] = { 0, 1, 2 };

	Ref<IndexBuffer> ib = CreateRef<IndexBuffer>(indices, sizeof(indices) / sizeof(uint32_t));

	m_VertexArray = CreateRef<VertexArray>();

	m_VertexArray->SetVertexBuffer(vb);
	m_VertexArray->SetIndexBuffer(ib);
}

void GameLayer::OnDetach()
{
	std::cout << "Game Layer: Layer detached" << std::endl;
}

void GameLayer::OnUpdate(float dt)
{
	m_VertexArray->Bind();
	glDrawElements(GL_TRIANGLES, m_VertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
}

void GameLayer::OnEvent(Event& e)
{
	EventDispatcher dispatcher(e);

	dispatcher.Dispatch<KeyPressedEvent>(BIND_FN(GameLayer::OnKeyPressed));
}

bool GameLayer::OnKeyPressed(KeyPressedEvent& e)
{
	std::cout << "Game Layer: " << e.ToString() << std::endl;
	return false;
}