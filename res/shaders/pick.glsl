#begin VERTEX



layout (location = 0) in vec3 position;

uniform mat4 projection = mat4(1.0);
uniform mat4 view = mat4(1.0);
uniform mat4 transform = mat4(1.0);

void main() {
	gl_Position = projection * view * transform * vec4(position, 1.0);
}

#end VERTEX

#begin FRAGMENT



out vec4 out_color;

uniform vec3 color;

void main() {
	out_color = vec4(color, 1.0);
}

#end FRAGMENT
