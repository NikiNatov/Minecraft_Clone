#include "pch.h"
#include "Shader.h"

#include <glm\gtc\type_ptr.hpp>

#include <glad\glad.h>

Shader::Shader(const std::string& filepath)
{
	const ShaderSource& src = ParseFile(filepath);
	CompileShader(src);

	size_t lastSlashIndex = filepath.find_last_of("/\\");
	lastSlashIndex = lastSlashIndex == std::string::npos ? 0 : lastSlashIndex + 1;
	size_t dotIndex = filepath.find_last_of(".");
	m_Name = filepath.substr(lastSlashIndex, dotIndex - lastSlashIndex);
}

Shader::~Shader()
{
	glDeleteProgram(m_RendererID);
}

void Shader::Bind()
{
	glUseProgram(m_RendererID);
}

void Shader::Unbind()
{
	glUseProgram(0);
}

void Shader::SetInt(const std::string& name, int value)
{
	glUseProgram(m_RendererID);
	int location = GetUniformLocation(name);
	glUniform1i(location, value);
}

void Shader::SetInt2(const std::string& name, const glm::ivec2& value)
{
	glUseProgram(m_RendererID);
	int location = GetUniformLocation(name);
	glUniform2i(location, value.x, value.y);
}

void Shader::SetInt3(const std::string& name, const glm::ivec3& value)
{
	glUseProgram(m_RendererID);
	int location = GetUniformLocation(name);
	glUniform3i(location, value.x, value.y, value.z);
}

void Shader::SetInt4(const std::string& name, const glm::ivec4& value)
{
	glUseProgram(m_RendererID);
	int location = GetUniformLocation(name);
	glUniform4i(location, value.x, value.y, value.z, value.w);
}

void Shader::SetFloat(const std::string& name, float value)
{
	glUseProgram(m_RendererID);
	int location = GetUniformLocation(name);
	glUniform1f(location, value);
}

void Shader::SetFloat2(const std::string& name, const glm::vec2& value)
{
	glUseProgram(m_RendererID);
	int location = GetUniformLocation(name);
	glUniform2f(location, value.x, value.y);
}

void Shader::SetFloat3(const std::string& name, const glm::vec3& value)
{
	glUseProgram(m_RendererID);
	int location = GetUniformLocation(name);
	glUniform3f(location, value.x, value.y, value.z);
}

void Shader::SetFloat4(const std::string& name, const glm::vec4& value)
{
	glUseProgram(m_RendererID);
	int location = GetUniformLocation(name);
	glUniform4f(location, value.x, value.y, value.z, value.w);
}

void Shader::SetBool(const std::string& name, bool value)
{
	glUseProgram(m_RendererID);
	int location = GetUniformLocation(name);
	glUniform1i(location, value);
}

void Shader::SetMat3(const std::string& name, int count, const glm::mat3& value)
{
	glUseProgram(m_RendererID);
	int location = GetUniformLocation(name);
	glUniformMatrix3fv(location, count, GL_FALSE, glm::value_ptr(value));
}

void Shader::SetMat4(const std::string& name, int count, const glm::mat4& value)
{
	glUseProgram(m_RendererID);
	int location = GetUniformLocation(name);
	glUniformMatrix4fv(location, count, GL_FALSE, glm::value_ptr(value));
}

int Shader::GetUniformLocation(const std::string& name)
{
	int location;
	if (m_UniformLocations.find(name) != m_UniformLocations.end())
		location = m_UniformLocations[name];
	else
	{
		location = glGetUniformLocation(m_RendererID, name.c_str());
		m_UniformLocations[name] = location;
	}
	return location;
}

ShaderSource Shader::ParseFile(const std::string& filepath)
{
	std::stringstream ss[2];
	std::ifstream stream(filepath, std::ios::in | std::ios::binary);

	std::string currentLine;

	enum class ShaderType { VertexShader = 0, FragmentShader = 1 };
	ShaderType type;

	if (stream.is_open())
	{
		while (std::getline(stream, currentLine))
		{
			if (currentLine.find("#shader") != std::string::npos)
			{
				if (currentLine.find("vertex") != std::string::npos)
					type = ShaderType::VertexShader;
				else if (currentLine.find("fragment") != std::string::npos)
					type = ShaderType::FragmentShader;
				else
					ASSERT(false, "Unknown shader type");
			}
			else
				ss[(int)type] << currentLine << '\n';
		}
	}
	else
		ASSERT(false, "Failed to open shader file!");


	return {ss[0].str(), ss[1].str()};
}

void Shader::CompileShader(const ShaderSource& source)
{
	uint32_t vertexShader = glCreateShader(GL_VERTEX_SHADER);
	const char* vertexSrc = source.VertexSource.c_str();
	glShaderSource(vertexShader, 1, &vertexSrc, 0);
	glCompileShader(vertexShader);

	int result = 0;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);

	if (result == GL_FALSE)
	{
		int maxLength = 0;
		glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

		char* infoLog = (char*)alloca(maxLength * sizeof(char));
		glGetShaderInfoLog(vertexShader, maxLength, &maxLength, infoLog);

		std::cout << "Vertex shader compilation failure: " << infoLog << std::endl;

		glDeleteShader(vertexShader);
		return;
	}

	uint32_t fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	const char* fragmentSrc = source.FragmentSource.c_str();
	glShaderSource(fragmentShader, 1, &fragmentSrc, 0);
	glCompileShader(fragmentShader);

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);

	if (result == GL_FALSE)
	{
		int maxLength = 0;
		glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

		char* infoLog = (char*)alloca(maxLength * sizeof(char));
		glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, infoLog);

		std::cout << "Fragment shader compilation failure: " << infoLog << std::endl;

		glDeleteShader(fragmentShader);
		return;
	}

	uint32_t program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);

	glGetProgramiv(program, GL_LINK_STATUS, &result);

	if (result == GL_FALSE)
	{
		int maxLength = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

		char* infoLog = (char*)alloca(maxLength * sizeof(char));
		glGetProgramInfoLog(program, maxLength, &maxLength, infoLog);

		std::cout << "Program linking failure: " << infoLog << std::endl;

		glDeleteProgram(program);
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		return;
	}

	m_RendererID = program;
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}
