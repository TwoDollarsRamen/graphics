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

uniform sampler2D depth;
uniform sampler2D normals;
uniform sampler2D input_texture;
uniform vec2 screen_size;

float calc_edge_depth(vec2 bl, vec2 tr, vec2 br, vec2 tl) {
	float d1 = texture(depth, bl).r;
	float d2 = texture(depth, tr).r;
	float d3 = texture(depth, br).r;
	float d4 = texture(depth, tl).r;

	float diff1 = d2 - d1;
	float diff2 = d4 - d3;

	float edge_depth = sqrt(pow(diff1, 2.0) + pow(diff2, 2.0)) * 100;
	edge_depth = edge_depth > 6.0 ? 1.0 : 0.0;

	return edge_depth;
}

float calc_edge_normal(vec2 bl, vec2 tr, vec2 br, vec2 tl) {
	vec3 d1 = texture(normals, bl).rgb;
	vec3 d2 = texture(normals, tr).rgb;
	vec3 d3 = texture(normals, br).rgb;
	vec3 d4 = texture(normals, tl).rgb;

	vec3 diff1 = d2 - d1;
	vec3 diff2 = d4 - d3;

	float edge_normal = sqrt(dot(diff1, diff1) + dot(diff2, diff2));
	edge_normal = edge_normal > 0.1 ? 1.0 : 0.0;

	return edge_normal;
}

void main() {
	float scale = 1.0;

	vec2 texel_size = 1.0 / screen_size;

	float half_scale_floor = floor(scale * 0.5);
	float half_scale_ceil = ceil(scale * 0.5);

	vec2 bl = fs_in.uv - texel_size * half_scale_floor;
	vec2 tr = fs_in.uv + texel_size * half_scale_ceil;
	vec2 br = fs_in.uv + texel_size * vec2(half_scale_ceil, half_scale_floor);
	vec2 tl = fs_in.uv + texel_size * vec2(half_scale_floor, half_scale_ceil);

	float edge_depth = calc_edge_depth(bl, tr, br, tl);
	float edge_normal = calc_edge_normal(bl, tr, br, tl);

	color = texture(input_texture, fs_in.uv) - vec4(vec3(max(edge_depth, edge_normal)), 0.0);
}

#end FRAGMENT
