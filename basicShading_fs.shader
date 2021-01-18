#version 330 core

// Interpolated values from the vertex shaders
in vec2 UV;
in vec3 Position_worldspace;
in vec3 Normal_cameraspace;
in vec3 EyeDirection_cameraspace;
in vec3 LightDirection_cameraspace;

// Ouput data
out vec3 color;

// Values that stay constant for the whole mesh.
uniform sampler2D myTextureSampler;
uniform vec3 LightPosition_worldspace;

void main() {

	vec3 LightColor = vec3(1, 1, 1);
	float LightPower = 10.0f;

	vec3 MaterialDiffuseColor = texture(myTextureSampler, UV).rgb;

	float distance = length(LightPosition_worldspace - Position_worldspace);

	vec3 n = normalize(Normal_cameraspace);
	vec3 l = normalize(LightDirection_cameraspace);
	float cosTheta = clamp(dot(n, l), 0, 1);

	color = (MaterialDiffuseColor * LightColor * LightPower * cosTheta / (distance * distance)) + // Á¤Áö ÅÂ¾ç±¤
		(MaterialDiffuseColor * 0.33); // ÁÖº¯±¤
}