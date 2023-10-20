
#version 460 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 textCoords_;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

uniform mat4 models[5];




out int draw;
out vec2 textCoords;


void main() {
	draw = gl_DrawID;
	textCoords = textCoords_;

	// copy the matrix and negate the x component for the transforms
	mat4 model_ = model;
	model_[3][0] *= -1;

	gl_Position = projection * view * model_ * vec4(pos, 1);
	//gl_Position = projection * vec4(pos.x+10, pos.y-10, pos.z-10, 1);
	//gl_Position = projection * view * models[gl_DrawID] * vec4(pos, 1);

}