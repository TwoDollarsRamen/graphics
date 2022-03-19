#begin VERTEX

#version 450 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uv;
layout (location = 2) in vec3 normal;

uniform mat4 projection = mat4(0.0);
uniform mat4 view = mat4(0.0);
uniform mat4 transform = mat4(0.0);

uniform float time;

out VS_OUT {
	vec3 normal;
} vs_out;

void main() {
	vs_out.normal = (transform * vec4(normal, 0.0)).xyz;

	gl_Position = projection * view * transform * vec4(position.x + position.y * sin(time), position.yz, 1.0);
}

#end VERTEX

#begin FRAGMENT

#version 450 core

out vec4 color;

in VS_OUT {
	vec3 normal;
} fs_in;

void main() {
	color = vec4(1.0) * clamp(dot(vec3(0.0, 0.0f, 1.0), fs_in.normal), 0.0, 1.0);
}

#end FRAGMENT
