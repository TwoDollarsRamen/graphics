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

uniform vec2 screen_size;

uniform sampler2D noise;
uniform vec3 samples[64];

uniform sampler2D positions;
uniform sampler2D normals;

uniform mat4 projection;

int kernel_size = 64;
float radius = 0.5;
float bias = 0.025;

void main() {
	vec2 noise_scale = vec2(
		screen_size.x / 4.0,
		screen_size.y / 4.0);

	vec3 frag_pos  = texture(positions, fs_in.uv).xyz;
	vec3 normal    = texture(normals, fs_in.uv).xyz;
	vec3 random    = texture(noise, fs_in.uv * noise_scale).xyz;

	vec3 tangent   = normalize(random - normal * dot(random, normal));
	vec3 bitangent = cross(normal, tangent);
	mat3 tbn       = mat3(tangent, bitangent, normal);  

	float occlusion = 0.0;
	for (int i = 0; i < kernel_size; i++) {
		vec3 sample_pos = tbn * samples[i];
		sample_pos = frag_pos + sample_pos * radius;

		vec4 off = vec4(sample_pos, 1.0);
		off = projection * off;
		off.xyz /= off.w;
		off.xyz  = off.xyz * 0.5 + 0.5;

		float sample_depth = texture(positions, off.xy).z;

		float range_check = smoothstep(0.0, 1.0, radius / abs(frag_pos.z - sample_depth));
		occlusion += (sample_depth >= sample_pos.z + bias ? 1.0 : 0.0) * range_check;
	}

	occlusion = 1.0 - (occlusion / kernel_size);

	//occlusion = 1.0;

	color = vec4(vec3(occlusion), 1.0);
	color = vec4(1.0);
}

#end FRAGMENT
