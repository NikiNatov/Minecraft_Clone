#shader vertex
#version 410 core

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec2 a_TexCoords;

out vec3 v_Position;
out vec2 v_TexCoords;

void main()
{
	v_Position = a_Position;
	v_TexCoords = a_TexCoords;
	gl_Position = vec4(a_Position, 1.0);
}

#shader fragment
#version 410 core

out vec4 o_Color;

in vec3 v_Position;
in vec2 v_TexCoords;

uniform sampler2D u_Texture;

void main()
{
	o_Color = texture(u_Texture, v_TexCoords);
}