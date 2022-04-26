#begin VERTEX

layout (location = 0) in vec3 position;

uniform mat4 light_matrix = mat4(1.0);
uniform mat4 transform = mat4(1.0);

void main() {
	gl_Position = light_matrix * transform * vec4(position, 1.0);
}

#end VERTEX

#begin FRAGMENT

void main() {}

#end FRAGMENT
