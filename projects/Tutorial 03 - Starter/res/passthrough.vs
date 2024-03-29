#version 410

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec4 inColor;

layout (location = 0) out vec4 outColor;

void main() {
	outColor = inColor;
	gl_Position = vec4(inPosition, 1);
}