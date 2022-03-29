#begin VERTEX

#version 450 core

layout (location = 0) in vec2 position;
layout (location = 1) in vec2 uv;

out VS_OUT {
	vec2 uv;
} vs_out;

void main() {
	vs_out.uv = uv;

	gl_Position = vec4(position, 0.0, 1.0);
}

#end VERTEX

#begin FRAGMENT

#version 450 core

in VS_OUT {
	vec2 uv;
} fs_in;

out vec4 out_color;

uniform sampler2D original_texture;
uniform sampler2D input_texture;
uniform vec2 screen_size;

void main() {
	vec3 i = texture(input_texture, fs_in.uv).rgb;
	vec3 o = texture(original_texture, fs_in.uv).rgb;

	out_color = vec4(i + o, 1.0);
}

#end FRAGMENT
