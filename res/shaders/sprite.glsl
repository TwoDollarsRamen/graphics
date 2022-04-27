#begin VERTEX

layout (location = 0) in vec2 position;
layout (location = 1) in vec2 uv;
layout (location = 2) in vec3 color;

uniform mat4 projection = mat4(1.0);

out VS_OUT {
	vec2 uv;
	vec3 color;
} vs_out;

void main() {
	vs_out.uv = uv;
	vs_out.color = color;

	gl_Position = projection * vec4(position, 0.0, 1.0);
}

#end VERTEX

#begin FRAGMENT

out vec4 color;

uniform sampler2D image;
uniform bool use_image;

in VS_OUT {
	vec2 uv;
	vec3 color;
} fs_in;

void main() {
	vec4 image_color = vec4(1.0);
	if (use_image) {
		image_color = texture(image, fs_in.uv);
	}

	color = image_color * vec4(fs_in.color, 1.0);
}

#end FRAGMENT
