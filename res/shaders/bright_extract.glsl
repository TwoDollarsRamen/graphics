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
uniform sampler2D ignore;
uniform vec2 screen_size;

void main() {
	vec4 texture_color = texture(input_texture, fs_in.uv) - texture(ignore, fs_in.uv);

	float pixel_brightness = dot(texture_color.rgb, vec3(0.2126, 0.7152, 0.0722));

	if (pixel_brightness > 0.7) {
		out_color = texture_color;
	} else {
		out_color = vec4(0.0, 0.0, 0.0, 1.0);
	}
}

#end FRAGMENT
