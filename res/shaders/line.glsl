#begin VERTEX

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;

uniform mat4 projection = mat4(1.0);
uniform mat4 view = mat4(1.0);

out VS_OUT {
	vec3 color;
} vs_out;

void main() {
	vs_out.color = color;

	gl_Position = projection * view * vec4(position, 1.0);
}

#end VERTEX

#begin FRAGMENT

out vec4 color;

in VS_OUT {
	vec3 color;
} fs_in;

void main() {
	color = vec4(fs_in.color, 1.0);
}

#end FRAGMENT
