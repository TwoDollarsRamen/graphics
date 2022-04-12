#begin VERTEX

#version 450 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uv;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitangent;

uniform mat4 projection = mat4(1.0);
uniform mat4 view = mat4(1.0);
uniform mat4 transform = mat4(1.0);

out VS_OUT {
	mat3 tbn;
	vec2 uv;
	vec3 position;
} vs_out;

void main() {
	vec3 t = normalize(vec3(transform * vec4(tangent, 0.0)));
	vec3 n = normalize(vec3(transform * vec4(normal, 0.0)));
	vec3 b = normalize(vec3(transform * vec4(bitangent, 0.0)));
	
	vs_out.tbn = mat3(t, b, n);

	vs_out.uv = uv;

	vec4 view_pos = view * transform * vec4(position, 1.0);
	vs_out.position = view_pos.xyz;

	gl_Position = projection * view * transform * vec4(position, 1.0);
}

#end VERTEX

#begin FRAGMENT

#version 450 core

layout (location = 0) out vec4 normal;
layout (location = 1) out vec4 position;

in VS_OUT {
	mat3 tbn;
	vec2 uv;
	vec3 position;
} fs_in;

uniform bool use_normal_map = false;
uniform sampler2D normal_map;

void main() {
	vec3 n = normalize(fs_in.tbn[2]);
	if (use_normal_map) {
		n = texture(normal_map, fs_in.uv).rgb * 2.0 - 1.0;
		n = normalize(fs_in.tbn * n);
	}

	normal = vec4(n, 1.0);
	position = vec4(fs_in.position, 1.0);
}

#end FRAGMENT
