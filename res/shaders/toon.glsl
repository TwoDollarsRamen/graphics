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

uniform sampler2D original_texture;
uniform sampler2D ignore;
uniform sampler2D input_texture;
uniform vec2 screen_size;

vec3 palette[] = {
	vec3(3, 16, 3)        / vec3(255),
	vec3(157, 157, 157)  / vec3(255),
	vec3(255, 255, 255)  / vec3(255),
	vec3(190, 38, 51)    / vec3(255),
	vec3(224, 111, 139)  / vec3(255),
	vec3(73, 60, 43)     / vec3(255),
	vec3(164, 100, 34)   / vec3(255),
	vec3(235, 137, 49)   / vec3(255),
	vec3(247, 226, 107)  / vec3(255),
	vec3(47, 72, 78)     / vec3(255),
	vec3(68, 137, 26)    / vec3(255),
	vec3(163, 206, 39)   / vec3(255),
	vec3(27, 38, 50)     / vec3(255),
	vec3(0, 87, 132)     / vec3(255),
	vec3(49, 162, 242)   / vec3(255),
	vec3(178, 220, 239)  / vec3(255),
};

void main() {
	vec4 tc = texture(input_texture, fs_in.uv) - texture(ignore, fs_in.uv);

	float smallest = 100000000000000000000000000.0;
	int index = 0;
	
	for (int i = 0; i < 16; i++) {
		float dist = length(vec3(pow(palette[i] - tc.rgb, vec3(2.0))));
		
		if (dist < smallest) {
			smallest = dist;
			index = i;
		}
	}

	color = vec4(palette[index], tc.a) + texture(ignore, fs_in.uv) * texture(original_texture, fs_in.uv);
}

#end FRAGMENT
