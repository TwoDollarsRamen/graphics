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

/* FXAA */
vec4 get_antialiased_color() {
	const float span_max = 4.0;
	const float reduce_min = 1.0/128.0;
	const float reduce_mul = 1.0/80.0;
	
	const vec2 texel_size = 1.0 / screen_size;
	const vec3 luma = vec3(0.299, 0.587, 0.114);

	float luma_TL = dot(luma, texture(input_texture, fs_in.uv + (vec2(-1.0, -1.0) * texel_size)).rgb);
	float luma_TR = dot(luma, texture(input_texture, fs_in.uv + (vec2(1.0, -1.0) * texel_size)).rgb);
	float luma_BL = dot(luma, texture(input_texture, fs_in.uv + (vec2(-1.0, 1.0) * texel_size)).rgb);
	float luma_BR = dot(luma, texture(input_texture, fs_in.uv + (vec2(1.0, 1.0) * texel_size)).rgb);
	float luma_M  = dot(luma, texture(input_texture, fs_in.uv).rgb);

	vec2 dir;
	dir.x = -((luma_TL + luma_TR) - (luma_BL + luma_BR));
	dir.y = ((luma_TL + luma_BL) - (luma_TR + luma_BR));

	float dir_reduce = max((luma_TL + luma_TR + luma_BL + luma_BR)
		* (reduce_mul * 0.25), reduce_min);
	float inverse_dir_adjustment = 1.0 / (min(abs(dir.x), abs(dir.y)) + dir_reduce);

	dir = min(vec2(span_max),
		max(vec2(-span_max), dir * inverse_dir_adjustment)) * texel_size;

	vec3 result1 = (1.0 / 2.0) * (
		texture(input_texture, fs_in.uv + (dir * vec2(1.0 / 3.0 - 0.5))).rgb +
		texture(input_texture, fs_in.uv + (dir * vec2(2.0 / 3.0 - 0.5))).rgb);

	vec3 result2 = result1 * (1.0 / 2.0 ) + (1.0 / 4.0) * (
		texture(input_texture, fs_in.uv + (dir * vec2(0.0 / 3.0 - 0.5))).rgb +
		texture(input_texture, fs_in.uv + (dir * vec2(3.0 / 3.0 - 0.5))).rgb);

	float luma_min = min(luma_M, min(min(luma_TL, luma_TR), min(luma_BL, luma_BR)));
	float luma_max = max(luma_M, max(max(luma_TL, luma_TR), max(luma_BL, luma_BR)));
	float luma_result2 = dot(luma, result2);

	if (luma_result2 < luma_min || luma_result2 > luma_max) {
		return vec4(result1, 1.0);
	}

	return vec4(result2, 1.0);
}


void main() {
	out_color = get_antialiased_color();
}

#end FRAGMENT
