#begin VERTEX



layout (location = 0) in vec2 position;
layout (location = 1) in vec2 uv;

out VS_OUT {
	vec2 uv;
} vs_out;

void main() {
	vs_out.uv = uv;

	gl_Position = vec4(position, 0.0, 1.0);
}

#end VERTEX

#begin FRAGMENT



in VS_OUT {
	vec2 uv;
} fs_in;

out vec4 color;

uniform sampler2D input_texture;
uniform vec2 screen_size;

void main() {
	vec4 tc = texture(input_texture, fs_in.uv);

	color = vec4(1.0 - tc.rgb, tc.a);
}

#end FRAGMENT
