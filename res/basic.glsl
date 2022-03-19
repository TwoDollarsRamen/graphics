#begin VERTEX

#version 450 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uv;
layout (location = 2) in vec3 normal;

uniform mat4 projection = mat4(0.0);
uniform mat4 view = mat4(0.0);
uniform mat4 transform = mat4(0.0);

out VS_OUT {
	vec3 normal;
	vec2 uv;
} vs_out;

void main() {
	vs_out.normal = (transform * vec4(normal, 0.0)).xyz;
	vs_out.uv = uv;

	gl_Position = projection * view * transform * vec4(position, 1.0);
}

#end VERTEX

#begin FRAGMENT

#version 450 core

out vec4 color;

in VS_OUT {
	vec3 normal;
	vec2 uv;
} fs_in;

uniform bool use_diffuse_map = false;
uniform sampler2D diffuse_map;

void main() {
	vec4 texture_color = vec4(1.0);
	if (use_diffuse_map) {
		texture_color = texture(diffuse_map, fs_in.uv);
	}

	color = texture_color * clamp(dot(vec3(0.0, 0.0f, 1.0), fs_in.normal), 0.0, 1.0);
}

#end FRAGMENT
