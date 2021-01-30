#pragma once

#include <glm\glm.hpp>

#include "Core\Base.h"

struct ShaderSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

class Shader
{
public:
	Shader(const std::string& filepath);
	~Shader();

	void Bind();
	void Unbind();

	void SetInt(const std::string& name, int value);
	void SetInt2(const std::string& name, const glm::ivec2& value);
	void SetInt3(const std::string& name, const glm::ivec3& value);
	void SetInt4(const std::string& name, const glm::ivec4& value);
	void SetFloat(const std::string& name, float value);
	void SetFloat2(const std::string& name, const glm::vec2& value);
	void SetFloat3(const std::string& name, const glm::vec3& value);
	void SetFloat4(const std::string& name, const glm::vec4& value);
	void SetBool(const std::string& name, bool value);
	void SetMat3(const std::string& name, int count, const glm::mat3& value);
	void SetMat4(const std::string& name, int count, const glm::mat4& value);

private:
	int GetUniformLocation(const std::string& name);
	ShaderSource ParseFile(const std::string& filepath);
	void CompileShader(const ShaderSource& source);
private:
	uint32_t m_RendererID;
	std::unordered_map<std::string, int> m_UniformLocations;
	std::string m_Name;
};