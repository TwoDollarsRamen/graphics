#begin VERTEX

layout (location = 0) in vec3 position;

void main() {
	gl_Position = vec4(position, 1.0);
}

#end VERTEX

#begin FRAGMENT

out vec4 color;

void main() {
	color = vec4(1.0);
}

#end FRAGMENT