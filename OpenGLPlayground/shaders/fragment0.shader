#version 460 core

out vec4 FragColor;

uniform sampler2D textures[32];




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
	vec3 position;
	// does position mean position or direction
	int directional;
};

Light light = { {.3,.3,.3}, {.75,.75,.75}, {.25,.25,.25}, {-1,-1,-1}, 1 };

vec3 calcLight() {
	vec3 ambient = light.ambient * material.albedo;

	vec3 norm = normalize(normal_);
	vec3 lightDir = normalize(-light.position);
	float diff = max(dot(norm, lightDir), 0);
	vec3 diffuse = light.diffuse * (diff * material.diffuse);


	vec3 result = (ambient + (diffuse));
	return result;
}

void main() {

	if (material.areTextures == 1) {
		FragColor = texture(textures[int(material.albedo.x)], textCoord_);
		//FragColor = texture(textures[0], textCoord_);

		//FragColor = vec4(material.albedo.x/10.0, 1, 1, 1);
	}
	else {
		FragColor = vec4(material.albedo,1);
	}


	//FragColor = vec4(1, 1, 1, 1);

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

	//FragColor = vec4(calcLight(), 1);

	//FragColor = vec4(abs(normal_.x), abs(normal_.y), abs(normal_.z), 1);
	//FragColor = vec4(textCoord_.x, textCoord_.y, 1, 1);
}
