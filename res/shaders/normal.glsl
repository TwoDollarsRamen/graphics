#begin VERTEX

#version 450 core

layout (location = 0) in vec3 position;
layout (location = 2) in vec3 normal;

uniform mat4 projection = mat4(1.0);
uniform mat4 view = mat4(1.0);
uniform mat4 transform = mat4(1.0);

out VS_OUT {
	vec3 normal;
} vs_out;

void main() {
	vs_out.normal = normalize(vec3(projection * view * transform * vec4(normal, 0.0)));

	gl_Position = projection * view * transform * vec4(position, 1.0);
}

#end VERTEX

#begin FRAGMENT

#version 450 core

out vec4 color;

in VS_OUT {
	vec3 normal;
} fs_in;

void main() {
	color = vec4(fs_in.normal * 0.5 * 0.5, 1.0);
}

#end FRAGMENT
