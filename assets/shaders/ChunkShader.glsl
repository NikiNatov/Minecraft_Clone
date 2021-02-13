#shader vertex
#version 410 core

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec2 a_TexCoords;
layout (location = 2) in vec3 a_Normal;
layout (location = 3) in float a_LightLevel;

out vec3 v_Position;
out vec2 v_TexCoords;
out float v_LightLevel;
out float v_Visibility;

uniform mat4 u_View;
uniform mat4 u_Projection;
uniform mat4 u_Transform;

const float FOG_DENSITY = 0.007;
const float FOG_GRADIENT = 5.5;

void main()
{
	v_Position = a_Position;
	v_TexCoords = a_TexCoords;
	v_LightLevel = a_LightLevel;

	vec4 positionRelativeToCamera = u_View * u_Transform *vec4(a_Position, 1.0);
	float distance = length(positionRelativeToCamera.xyz);
	v_Visibility = exp(-pow(distance * FOG_DENSITY, FOG_GRADIENT));

	gl_Position = u_Projection * u_View * u_Transform * vec4(a_Position, 1.0);
}

#shader fragment
#version 410 core

out vec4 o_Color;

in vec3 v_Position;
in vec2 v_TexCoords;
in float v_LightLevel;
in float v_Visibility;

uniform sampler2D u_Texture;

void main()
{
	o_Color = v_LightLevel * texture(u_Texture, v_TexCoords);
	o_Color = mix(vec4(0.4, 0.7, 1.0, 1.0), o_Color, v_Visibility);
}