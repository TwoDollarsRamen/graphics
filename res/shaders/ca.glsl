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

out vec4 out_color;

uniform sampler2D input_texture;
uniform vec2 screen_size;

void main() {
	/* Sort of chromatic aberration. */
	vec4 tex1 = texture(input_texture, fs_in.uv + vec2(sin(fs_in.uv.y * 150 * 0.2) * 0.004));
	vec4 tex2 = texture(input_texture, fs_in.uv + vec2(sin(fs_in.uv.y * 100 * 0.2) * 0.004));
	vec4 tex3 = texture(input_texture, fs_in.uv + vec2(sin(fs_in.uv.y * 200 * 0.2) * 0.004));

	out_color = vec4(tex1.r, tex2.g, tex3.b, 1.0);
}

#end FRAGMENT
