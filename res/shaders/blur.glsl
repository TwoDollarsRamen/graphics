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

uniform sampler2D input_texture;
uniform vec2 screen_size;

void main() {
	float blur_amt = 0.2;

	vec4 accum = vec4(0.0);
	float tap_count = 0;

	for (float x = -0.1; x <= 0.1; x += 0.01) {
		for (float y = -0.1; y <= 0.1; y += 0.01) {
			accum += texture(input_texture, fs_in.uv + vec2(x, y) * blur_amt);
			tap_count++;
		}
	}

	out_color = accum / tap_count;
}

#end FRAGMENT
