#begin VERTEX

#version 450 core

layout (location = 0) in vec2 position;
layout (location = 1) in vec2 uv;

uniform mat4 projection = mat4(1.0);

out VS_OUT {
	vec2 uv;
} vs_out;

void main() {
	vs_out.uv = uv;

	gl_Position = projection * vec4(position, 0.0, 1.0);
}

#end VERTEX

#begin FRAGMENT

#version 450 core

out vec4 color;

uniform sampler2D image;

in VS_OUT {
	vec2 uv;
} fs_in;

void main() {
	color = texture(image, fs_in.uv);
}

#end FRAGMENT
