#begin VERTEX



layout (location = 0) in vec2 position;
layout (location = 1) in vec2 uv;

out VS_OUT {
	vec2 uv;
} vs_out;

uniform mat4 transform = mat4(1.0);
uniform mat4 camera = mat4(1.0);

void main() {
	vs_out.uv = uv;

	gl_Position = camera * transform * vec4(position, 0.0, 1.0);
}

#end VERTEX

#begin FRAGMENT



in VS_OUT {
	vec2 uv;
} fs_in;

out vec4 color;

uniform sampler2D t;

void main() {
	color = texture(t, fs_in.uv);
}

#end FRAGMENT
