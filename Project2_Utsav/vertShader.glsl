#version 330 core

layout(location = 0) in vec3 vertPosModel;
layout(location = 1) in vec3 normals;

out vec4 v_color;

uniform mat4 mvp;

void main()
{
	gl_Position = mvp * vec4((0.05*vertPosModel), 1.0);
	v_color = vec4(0.0, 1.0, 0.0, 1.0);
}
