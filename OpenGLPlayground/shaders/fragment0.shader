#version 450 core

out vec4 FragColor;

uniform sampler2D textures[32];

void main() {
	FragColor = vec4(1, 1, 0, 1);
}
