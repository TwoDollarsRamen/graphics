#begin VERTEX

#version 450 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uv;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 binormal;

uniform mat4 projection = mat4(0.0);
uniform mat4 view = mat4(0.0);
uniform mat4 transform = mat4(0.0);

out VS_OUT {
	vec2 uv;
	vec3 world_pos;
	mat3 tbn;
} vs_out;

void main() {
	vec3 t = normalize(vec3(transform * vec4(tangent, 0.0)));
	vec3 n = normalize(vec3(transform * vec4(normal, 0.0)));
	vec3 b = normalize(vec3(transform * vec4(binormal, 0.0)));
	
	vs_out.tbn = mat3(t, b, n);
	
	vs_out.uv = uv;
	vs_out.world_pos = vec3(transform * vec4(position, 1.0));

	gl_Position = projection * view * transform * vec4(position, 1.0);
}

#end VERTEX

#begin FRAGMENT

#version 450 core

out vec4 color;

in VS_OUT {
	vec2 uv;
	vec3 world_pos;
	mat3 tbn;
} fs_in;

uniform bool use_diffuse_map = false;
uniform sampler2D diffuse_map;

uniform bool use_specular_map = false;
uniform sampler2D specular_map;

uniform bool use_normal_map = false;
uniform sampler2D normal_map;

#define max_point_lights 32
#define max_directional_lights 3

struct PointLight {
	vec3 position;
	float range;
	vec3 specular, diffuse;
	float intensity;
};

struct DirectionalLight {
	float range;
	vec3 specular, diffuse;
	vec3 direction;
	float intensity;
};

struct Material {
	vec3 ambient, diffuse, specular;
	float shininess;
};

uniform Material material;

struct World {
	float ambient_intensity;
	vec3 ambient;
	vec3 camera_pos;
};

uniform World world;

uniform uint point_light_count = 0;
uniform PointLight point_lights[max_point_lights];

uniform uint directional_light_count = 0;
uniform DirectionalLight directional_lights[max_directional_lights];

vec3 compute_point_light(PointLight light, vec3 normal, vec3 view_dir, vec3 specular_map_color, vec3 diffuse_map_color) {
	vec3 light_dir = normalize(light.position - fs_in.world_pos);
	vec3 reflect_dir = reflect(-light_dir, normal);
	
	/* Attenuation from: https://geom.io/bakery/wiki/index.php?title=Point_Light_Attenuation
	 *
	 * The same attenuation model as Bakery uses. */
	float a = 1.0 / (pow(length(light.position - fs_in.world_pos), 2.0) + 1);
	
	vec3 diffuse = material.diffuse * diffuse_map_color * light.diffuse * light.intensity * max(dot(light_dir, normal), 0.0) * a;
	vec3 specular = material.specular * specular_map_color * light.specular * light.intensity * pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess) * a;

	return diffuse + specular;
}

vec3 compute_directional_light(DirectionalLight light, vec3 normal, vec3 view_dir, vec3 specular_map_color, vec3 diffuse_map_color) {
	vec3 light_dir = normalize(light.direction);
	vec3 reflect_dir = reflect(-light_dir, normal);
	
	vec3 diffuse = material.diffuse * diffuse_map_color * light.diffuse * light.intensity * max(dot(light_dir, normal), 0.0);
	vec3 specular = material.specular * specular_map_color * light.specular * light.intensity * pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);

	return diffuse + specular;
}

void main() {
	vec4 diffuse_map_color = vec4(1.0);
	if (use_diffuse_map) {
		diffuse_map_color = texture(diffuse_map, fs_in.uv);
	}

	vec4 specular_map_color = vec4(1.0);
	if (use_specular_map) {
		specular_map_color = texture(specular_map, fs_in.uv);
	}

	vec3 view_dir = normalize(world.camera_pos - fs_in.world_pos);
	
	vec3 n = normalize(fs_in.tbn[2]);
	if (use_normal_map) {
		n = texture(normal_map, fs_in.uv).rgb * 2.0 - 1.0;
		n = normalize(fs_in.tbn * n);
	}

	vec3 lighting_result = diffuse_map_color.rgb * world.ambient_intensity * world.ambient * material.ambient;
	for (uint i = 0; i < point_light_count; i++) {
		lighting_result += compute_point_light(point_lights[i], n, view_dir,
			specular_map_color.rgb, diffuse_map_color.rgb);
	}
	
	for (uint i = 0; i < directional_light_count; i++) {
		lighting_result += compute_directional_light(directional_lights[i], n, view_dir,
			specular_map_color.rgb, diffuse_map_color.rgb);
	}

	color = vec4(lighting_result, 1.0);
	//color = vec4(fs_in.tbn[1], 1.0);
}

#end FRAGMENT