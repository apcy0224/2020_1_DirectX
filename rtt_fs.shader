#version 330 core

in vec2 UV;
in vec3 Position_worldspace;
in vec3 Normal_cameraspace;
in vec3 EyeDirection_cameraspace;
in vec3 LightDirection_cameraspace;

out vec3 color;

uniform sampler2D renderedTexture;
uniform float time;
uniform vec3 LightPosition_worldspace;

void main() {
	vec3 LightColor = vec3(1, 1, 1);
	float LightPower = 5.0f;

	float distance = length(LightPosition_worldspace - Position_worldspace);

	vec3 n = normalize(Normal_cameraspace);
	vec3 l = normalize(LightDirection_cameraspace);
	float cosTheta = clamp(dot(n, l), 0, 1);

	color = (LightColor * LightPower * cosTheta / (distance * distance)) + // 정지 태양광
		(texture(renderedTexture, UV + 0.005*vec2(sin(time + 1024.0*UV.x), cos(time + 768.0*UV.y))).rgb // rtt 텍스처
		* vec3(1, 0, 0));
}