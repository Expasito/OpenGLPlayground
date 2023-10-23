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
	int areTextures;
};

uniform Material material;

void main() {

	if (material.areTextures == 1) {
		vec3 result = vec3(0);
		vec4 alb = texture(textures[int(material.albedo.x)], textCoords);
		result += alb.xyz;
		FragColor = vec4(result, 1);
	}
	else {
		vec3 result = vec3(0);
		result += material.albedo;
		FragColor = vec4(result,1);
	}
}
