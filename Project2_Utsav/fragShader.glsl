#version 330 core

in vec4 v_color;
out vec4 o_color;

uniform vec3 lightPositionWorld;
uniform vec3 ambientLight;
uniform vec3 cameraPositionWorld;

void main()
{
	//Diffuse Light
	vec4 newAmbientLight = vec4(ambientLight, 1.0f);
	vec3 newNormalWorld = normalize(normalTangent);
	vec3 lightVectorWorld = normalize(lightPositionWorld - vertexPositionWorld);
	float brightness = (dot(lightVectorWorld, newNormalWorld));
	vec4 diffuseLight = clamp(vec4(brightness, brightness, brightness, 1.0f), 0, 1);

	//attenuation
	float lightDistance = distance(lightPositionWorld, vertexPositionWorld);
	float attenuationConstant = 1.2f;
	float attenuationConstantLinear = 0.2;
	float attenuationConstantQuadratic = 0.1;
	float lightAttenuation = 1/(attenuationConstant + (attenuationConstantLinear * lightDistance) + (attenuationConstantQuadratic * pow(lightDistance, 2)));
	diffuseLight = lightAttenuation * diffuseLight;

	//Specular
	vec3 specularLightVectorWorld = reflect(-lightVectorWorld, normalTangent);
	vec3 cameraVectorWorld = normalize(cameraPositionWorld - vertexPositionWorld);
	float specular = dot(specularLightVectorWorld, cameraVectorWorld);
	specular = pow(specular, 40);
	vec4 specularity = clamp(vec4(specular, specular, specular, 1), 0, 1);

	o_color = v_color * (diffuseLight + newAmbientLight + specularity);
}
