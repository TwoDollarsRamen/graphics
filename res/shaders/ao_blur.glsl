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

out vec4 color;

uniform sampler2D input_texture;
uniform vec2 screen_size;

void main() {
	vec2 texel_size = 1.0 / vec2(textureSize(input_texture, 0));
	float result = 0.0;
	for (int x = -2; x < 2; x++) {
		for (int y = -2; y < 2; y++) {
			vec2 off = vec2(float(x), float(y)) * texel_size;
			result += texture(input_texture, fs_in.uv + off).r;
		}
	}

    color = vec4(result / (4.0 * 4.0));
}

#end FRAGMENT
