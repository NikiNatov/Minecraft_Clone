#include "pch.h"
#include "GameLayer.h"

#include "Core\Base.h"


GameLayer::GameLayer()
	: m_Camera({0.0f, 0.0f, 3.0}, 45.0f, 16.0f / 9.0f)
{
}

GameLayer::~GameLayer()
{
}

void GameLayer::OnAttach()
{
	float vertices[] =
	{
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		 0.5f,  0.5f, 0.0f,	1.0f, 1.0f,
		-0.5f,  0.5f, 0.0f,	0.0f, 1.0f
	};
	uint32_t indices[] = { 0, 1, 2, 2, 3, 0 };

	// Vertex buffer
	Ref<VertexBuffer> vb = CreateRef<VertexBuffer>(vertices, sizeof(vertices));
	vb->SetLayout(
	{
		{"a_Position", DataType::Float3, false},
		{"a_TexCoords", DataType::Float2, false},
	});

	//Index buffer
	Ref<IndexBuffer> ib = CreateRef<IndexBuffer>(indices, sizeof(indices) / sizeof(uint32_t));

	// Vertex array
	m_VertexArray = CreateRef<VertexArray>();
	m_VertexArray->SetVertexBuffer(vb);
	m_VertexArray->SetIndexBuffer(ib);

	// Shader
	m_TextureShader = CreateRef<Shader>("assets/shaders/TextureShader.glsl");

	// Texture
	m_Texture = CreateRef<Texture2D>("assets/textures/minecraft_spritesheet.png", TextureFilter::Linear, TextureFilter::Nearest, TextureWrap::Repeat, true);
}

void GameLayer::OnDetach()
{
}

void GameLayer::OnUpdate(float dt)
{
	m_Camera.OnUpdate(dt);

	m_TextureShader->Bind();
	m_TextureShader->SetMat4("u_Projection", 1, m_Camera.GetProjectionMatrix());
	m_TextureShader->SetMat4("u_View", 1, m_Camera.GetViewMatrix());

	m_Texture->Bind(0);
	m_VertexArray->Bind();
	glDrawElements(GL_TRIANGLES, m_VertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
}

void GameLayer::OnEvent(Event& e)
{
	EventDispatcher dispatcher(e);
	dispatcher.Dispatch<WindowResizedEvent>(BIND_FN(GameLayer::OnWindowResized));
}

bool GameLayer::OnWindowResized(WindowResizedEvent& e)
{
	m_Camera.SetProjection(45.0f, (float)e.GetWidth() / (float)e.GetHeight());
	return false;
}
