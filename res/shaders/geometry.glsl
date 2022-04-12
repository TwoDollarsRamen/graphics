#begin VERTEX

#version 450 core

layout (location = 0) in vec3 position;
layout (location = 2) in vec3 normal;

uniform mat4 projection = mat4(1.0);
uniform mat4 view = mat4(1.0);
uniform mat4 transform = mat4(1.0);

out VS_OUT {
	vec3 normal;
	vec3 position;
} vs_out;

void main() {
	vs_out.normal = normalize(vec3(transform * vec4(normal, 0.0)));

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
	vec3 normal;
	vec3 position;
} fs_in;

uniform bool use_normal_map = false;
uniform sampler2D normal_map;

void main() {
	normal = vec4(fs_in.normal, 1.0);
	position = vec4(fs_in.position, 1.0);
}

#end FRAGMENT
