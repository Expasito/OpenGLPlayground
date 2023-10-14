#version 460 core

out vec4 FragColor;

uniform sampler2D textures[32];

flat in int draw;

void main() {
	int test = draw + 5;
	FragColor = vec4(test/10.0, 1, 0, 1);
}
