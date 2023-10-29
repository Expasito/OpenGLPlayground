#version 460 core

out vec4 FragColor;

uniform sampler2D textures[32];

flat in int draw;

uniform int texturing;

in vec3 pos_;
in vec3 normal_;
in vec2 textCoord_;

struct Material {
	vec3 albedo;
	vec3 diffuse;
	vec3 specular;
	float shininess;
	int areTextures;
};

uniform Material material;

struct Light {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

Light light = { {0,0,0}, {.5,.5,.5}, {.25,.25,.25} };

vec3 calcLight(vec3 albedo, vec3 diffuse, vec3 specular, float shininess) {
	vec3 ambient = light.ambient * albedo;
	return ambient;
	// more lighting here
}

void main() {

	//if (material.areTextures == 1) {
	//	vec3 result = vec3(0);
	//	vec4 alb = texture(textures[int(material.albedo.x)], textCoords);
	//	result += alb.xyz;
	//	FragColor = vec4(result, 1);
	//}
	//else {
	//	vec3 result = vec3(0);
	//	result += material.albedo;
	//	FragColor = vec4(result,1);
	//}


	FragColor = vec4(abs(normal_.x), abs(normal_.y), abs(normal_.z), 1);
	//FragColor = vec4(textCoord_.x, textCoord_.y, 1, 1);
}
