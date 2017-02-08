#version 330 core

layout(location = 0) in vec3 vertPosModel;
layout(location = 1) in vec3 normalModel;

out vec4 v_color;

uniform mat4 modelToProjectionMatrix;
uniform mat4 modelToWorldMatrix;

void main()
{
	gl_Position = modelToProjectionMatrix; * vec4((vertPosModel), 1.0);
	normalWorld = normalize(vec3(modelToWorldMatrix * vec4(normalModel, 0)));
	v_color = vec4(0.0, 1.0, 0.0, 1.0);
}
