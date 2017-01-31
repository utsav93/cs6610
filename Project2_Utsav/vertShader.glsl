#version 330 core

layout(location = 0) in vec3 vertPosModel;

out vec4 v_color;

uniform mat4 mvp;

void main()
{
	gl_Position = mvp * vec4((0.05*vertPosModel), 1.0);
	v_color = vec4(1.0, 0.0, 0.0, 1.0);
}
