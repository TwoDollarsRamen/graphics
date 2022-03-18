#begin VERTEX

#version 450 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uv;
layout (location = 2) in vec3 normal;

void main() {
	gl_Position = vec4(position, 1.0);
}

#end VERTEX

#begin FRAGMENT

#version 450 core

out vec4 color;

void main() {
	color = vec4(0.0, 1.0, 0.0, 1.0);
}

#end FRAGMENT
