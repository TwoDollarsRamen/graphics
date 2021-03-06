#begin VERTEX



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



in VS_OUT {
	vec2 uv;
} fs_in;

out vec4 color;

uniform sampler2D input_texture;
uniform vec2 screen_size;

/* From: https://knarkowicz.wordpress.com/2016/01/06/aces-filmic-tone-mapping-curve/ */
vec3 aces(vec3 x) {
	float a = 2.51f;
	float b = 0.03f;
	float c = 2.43f;
	float d = 0.59f;
	float e = 0.14f;
	return clamp((x * (a * x + b)) / (x * (c * x + d) + e), 0.0, 1.0);
}

uniform sampler2D noise;

void main() {
	vec4 tc = texture(input_texture, fs_in.uv);

	color = vec4(aces(tc.rgb), tc.a);
}

#end FRAGMENT
