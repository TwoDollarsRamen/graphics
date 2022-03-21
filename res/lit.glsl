#begin VERTEX

#version 450 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uv;
layout (location = 2) in vec3 normal;

uniform mat4 projection = mat4(0.0);
uniform mat4 view = mat4(0.0);
uniform mat4 transform = mat4(0.0);

out VS_OUT {
	vec3 normal;
	vec2 uv;
	vec3 world_pos;
} vs_out;

void main() {
	vs_out.normal = mat3(transpose(inverse(transform))) * normal;
	vs_out.uv = uv;
	vs_out.world_pos = vec3(transform * vec4(position, 1.0));

	gl_Position = projection * view * transform * vec4(position, 1.0);
}

#end VERTEX

#begin FRAGMENT

#version 450 core

out vec4 color;

in VS_OUT {
	vec3 normal;
	vec2 uv;
	vec3 world_pos;
} fs_in;

uniform bool use_diffuse_map = false;
uniform sampler2D diffuse_map;

uniform bool use_specular_map = false;
uniform sampler2D specular_map;

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
	vec3 halfway_dir = normalize(light_dir + view_dir);

	float diff = max(dot(normal, light_dir), 0.0);

	vec3 reflect_dir = reflect(-light_dir, normal);
	float spec = pow(max(dot(normal, halfway_dir), 0.0), material.shininess);

	float dist = length(light.position - fs_in.world_pos);
	float attenuation = 1.0 / (pow((dist / light.range) * 5.0, 2.0) + 1.0);

	vec3 diffuse = light.diffuse * light.intensity * diff * diffuse_map_color * material.diffuse;
	vec3 specular = light.specular * light.intensity * spec * specular_map_color * material.specular;

	diffuse *= attenuation;
	specular *= attenuation;

	return diffuse + specular;
}

vec3 compute_directional_light(DirectionalLight light, vec3 normal, vec3 view_dir, vec3 specular_map_color, vec3 diffuse_map_color) {
    vec3 light_dir = normalize(-light.direction);
	vec3 halfway_dir = normalize(light_dir + view_dir);
	
    float diff = max(dot(normal, light_dir), 0.0);
    vec3 diffuse = light.diffuse * diff * diffuse_map_color * light.intensity;

    vec3 reflect_dir = reflect(light_dir, normal);
	float spec = pow(max(dot(normal, halfway_dir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * specular_map_color * light.intensity;

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

	vec3 lighting_result = diffuse_map_color.rgb * world.ambient_intensity * world.ambient * material.ambient;
	for (uint i = 0; i < point_light_count; i++) {
		lighting_result += compute_point_light(point_lights[i], fs_in.normal, view_dir,
			specular_map_color.rgb, diffuse_map_color.rgb);
	}
	
	for (uint i = 0; i < directional_light_count; i++) {
		lighting_result += compute_directional_light(directional_lights[i], fs_in.normal, view_dir,
			specular_map_color.rgb, diffuse_map_color.rgb);
	}

	color = vec4(lighting_result, 1.0);
}

#end FRAGMENT
