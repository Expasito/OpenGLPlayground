#version 460 core

out vec4 FragColor;

uniform sampler2D textures[32];

flat in int draw;

uniform int texturing;

in vec2 textCoords;

struct Material {
	vec3 albedo;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

uniform Material material;

void main() {

	//Material material = { {1,0,0}, {0,1,0}, {0,0,1}, 10 };

	int test = draw + 5;

	if (texturing >= 0) {
		FragColor = texture(textures[texturing],textCoords);
	}
	else {
		FragColor = vec4(0, 1, 0, 1);
	}

	FragColor = vec4(textCoords.x, textCoords.y, 1, 1);
	FragColor = vec4(test/10.0, 1, 0, 1);
	FragColor = vec4(material.albedo, 1);
}
