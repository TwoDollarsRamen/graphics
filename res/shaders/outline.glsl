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

uniform sampler2D normals;
uniform sampler2D input_texture;
uniform vec2 screen_size;

void main() {
	float scale = 1.0;

	vec2 texel_size = 1.0 / screen_size;

	float half_scale_floor = floor(scale * 0.5);
	float half_scale_ceil = ceil(scale * 0.5);

	vec2 bl = fs_in.uv - texel_size * half_scale_floor;
	vec2 tr = fs_in.uv + texel_size * half_scale_ceil;
	vec2 br = fs_in.uv + texel_size * vec2(half_scale_ceil, half_scale_floor);
	vec2 tl = fs_in.uv + texel_size * vec2(half_scale_floor, half_scale_ceil);

	vec3 n1 = texture(normals, bl).rgb;
	vec3 n2 = texture(normals, tr).rgb;
	vec3 n3 = texture(normals, br).rgb;
	vec3 n4 = texture(normals, tl).rgb;

	vec3 diff1 = n2 - n1;
	vec3 diff2 = n4 - n3;

	float edge_normal = sqrt(dot(diff1, diff1) + dot(diff2, diff2));
	edge_normal = edge_normal > 0.07 ? 1.0 : 0.0;

	color = texture(input_texture, fs_in.uv) - vec4(vec3(edge_normal), 0.0);
}

#end FRAGMENT
