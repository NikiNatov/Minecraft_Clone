#shader vertex
#version 410 core

layout (location = 0) in vec3 a_Position;

out vec3 v_Position;

void main()
{
	v_Position = a_Position;
	gl_Position = vec4(a_Position, 1.0);
}

#shader fragment
#version 410 core

out vec4 o_Color;

in vec3 v_Position;

void main()
{
	o_Color = vec4(v_Position * 0.45 + 0.5, 1.0);
}