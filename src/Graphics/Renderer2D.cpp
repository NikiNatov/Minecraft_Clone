#include "pch.h"
#include "Renderer2D.h"

#include <glad\glad.h>

#include "Graphics\Shader.h"
#include "Graphics\VertexArray.h"

struct QuadVertex
{
	glm::vec3 Position;
	glm::vec2 TexCoord;
};

struct Renderer2DData
{
	Ref<Shader> Shader;
	QuadVertex QuadVertices[4];
	Ref<VertexBuffer> QuadVB;
	Ref<IndexBuffer> QuadIB;
	Ref<VertexArray> QuadVA;

	glm::mat4 Projection;
};

static Ref<Renderer2DData> s_Data = CreateRef<Renderer2DData>();

void Renderer2D::Init()
{
	s_Data->Shader = CreateRef<Shader>("assets/shaders/UIShader.glsl");

	s_Data->QuadVertices[0].Position = { -0.5f, -0.5f, 0.0f };
	s_Data->QuadVertices[1].Position = {  0.5f, -0.5f, 0.0f };
	s_Data->QuadVertices[2].Position = {  0.5f,  0.5f, 0.0f };
	s_Data->QuadVertices[3].Position = { -0.5f,  0.5f, 0.0f };

	s_Data->QuadVertices[0].TexCoord = { 0.0f, 0.0f };
	s_Data->QuadVertices[1].TexCoord = { 1.0f, 0.0f };
	s_Data->QuadVertices[2].TexCoord = { 1.0f, 1.0f };
	s_Data->QuadVertices[3].TexCoord = { 0.0f, 1.0f };

	uint32_t indices[6] = { 0, 1, 2, 2, 3, 0 };

	BufferLayout layout = 
	{
		{"a_Position", DataType::Float3, false},
		{"a_TexCoord", DataType::Float2, false},
	};

	s_Data->QuadVB = CreateRef<VertexBuffer>(s_Data->QuadVertices, 4 * sizeof(QuadVertex));
	s_Data->QuadVB->SetLayout(layout);

	s_Data->QuadIB = CreateRef<IndexBuffer>(indices, 6);

	s_Data->QuadVA = CreateRef<VertexArray>();
	s_Data->QuadVA->Bind();
	s_Data->QuadVA->SetIndexBuffer(s_Data->QuadIB);
	s_Data->QuadVA->SetVertexBuffer(s_Data->QuadVB);
	s_Data->QuadVA->Unbind();
}

void Renderer2D::BeginScene(const glm::mat4& projection)
{
	s_Data->Projection = projection;
}

void Renderer2D::DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture)
{
	BufferLayout layout =
	{
		{"a_Position", DataType::Float3, false},
		{"a_TexCoord", DataType::Float2, false},
	};

	s_Data->QuadVertices[0].TexCoord = { 0.0f, 0.0f };
	s_Data->QuadVertices[1].TexCoord = { 1.0f, 0.0f };
	s_Data->QuadVertices[2].TexCoord = { 1.0f, 1.0f };
	s_Data->QuadVertices[3].TexCoord = { 0.0f, 1.0f };

	s_Data->QuadVB = CreateRef<VertexBuffer>(s_Data->QuadVertices, 4 * sizeof(QuadVertex));
	s_Data->QuadVB->SetLayout(layout);

	s_Data->QuadVA = CreateRef<VertexArray>();
	s_Data->QuadVA->Bind();
	s_Data->QuadVA->SetIndexBuffer(s_Data->QuadIB);
	s_Data->QuadVA->SetVertexBuffer(s_Data->QuadVB);
	s_Data->QuadVA->Unbind();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	s_Data->Shader->Bind();
	s_Data->Shader->SetMat4("u_Transform", 1, transform);
	s_Data->Shader->SetMat4("u_Projection", 1, s_Data->Projection);
	s_Data->Shader->SetInt("u_Texture", 0);
	texture->Bind(0);
	s_Data->QuadVA->Bind();
	glDrawElements(GL_TRIANGLES, s_Data->QuadIB->GetCount(), GL_UNSIGNED_INT, nullptr);
	s_Data->QuadVA->Unbind();
}

void Renderer2D::DrawQuad(const glm::mat4& transform, const Ref<SubTexture2D>& subtexture)
{
	BufferLayout layout =
	{
		{"a_Position", DataType::Float3, false},
		{"a_TexCoord", DataType::Float2, false},
	};

	s_Data->QuadVertices[0].TexCoord = subtexture->GetTextureCoords()[3];
	s_Data->QuadVertices[1].TexCoord = subtexture->GetTextureCoords()[2];
	s_Data->QuadVertices[2].TexCoord = subtexture->GetTextureCoords()[1];
	s_Data->QuadVertices[3].TexCoord = subtexture->GetTextureCoords()[0];

	s_Data->QuadVB = CreateRef<VertexBuffer>(s_Data->QuadVertices, 4 * sizeof(QuadVertex));
	s_Data->QuadVB->SetLayout(layout);

	s_Data->QuadVA = CreateRef<VertexArray>();
	s_Data->QuadVA->Bind();
	s_Data->QuadVA->SetIndexBuffer(s_Data->QuadIB);
	s_Data->QuadVA->SetVertexBuffer(s_Data->QuadVB);
	s_Data->QuadVA->Unbind();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	s_Data->Shader->Bind();
	s_Data->Shader->SetMat4("u_Transform", 1, transform);
	s_Data->Shader->SetMat4("u_Projection", 1, s_Data->Projection);
	s_Data->Shader->SetInt("u_Texture", 0);
	subtexture->GetTexture()->Bind(0);
	s_Data->QuadVA->Bind();
	glDrawElements(GL_TRIANGLES, s_Data->QuadIB->GetCount(), GL_UNSIGNED_INT, nullptr);
	s_Data->QuadVA->Unbind();
}
