
#version 460 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 textCoord;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

uniform mat4 models[6];




out int draw;
out vec3 pos_;
out vec3 normal_;
out vec2 textCoord_;


void main() {
	draw = gl_DrawID;

	pos_ = pos;
	normal_ = normal;
	//textCoord_ = vec2(textCoord.x, textCoord.y);
	textCoord_ = textCoord;
	//textCoords = textCoords_;

	// copy the matrix and negate the x component for the transforms
	mat4 model_ = model;

	//mat4 model_ = models[draw];
	model_[3][0] *= -1;

	gl_Position = projection * view * model_ * vec4(pos, 1);
	//gl_Position = projection * vec4(pos.x+10, pos.y-10, pos.z-10, 1);
	//gl_Position = projection * view * models[gl_DrawID] * vec4(pos, 1);

}