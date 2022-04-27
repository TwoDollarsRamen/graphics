#begin VERTEX

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uv;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitangent;

uniform mat4 projection = mat4(1.0);
uniform mat4 view = mat4(1.0);
uniform mat4 transform = mat4(1.0);
uniform mat4 light_matrix = mat4(1.0);

out VS_OUT {
	vec2 uv;
	vec3 world_pos;
	mat3 tbn;
	vec4 light_pos;
} vs_out;

void main() {
	vec3 t = normalize(vec3(transform * vec4(tangent, 0.0)));
	vec3 n = normalize(vec3(transform * vec4(normal, 0.0)));
	vec3 b = normalize(vec3(transform * vec4(bitangent, 0.0)));
	
	vs_out.tbn = mat3(t, b, n);
	
	vs_out.uv = uv;
	vs_out.world_pos = vec3(transform * vec4(position, 1.0));
	vs_out.light_pos = light_matrix * vec4(vs_out.world_pos, 1.0);

	gl_Position = projection * view * transform * vec4(position, 1.0);
}

#end VERTEX

#begin FRAGMENT

out vec4 color;

in VS_OUT {
	vec2 uv;
	vec3 world_pos;
	mat3 tbn;
	vec4 light_pos;
} fs_in;

uniform bool use_diffuse_map = false;
uniform sampler2D diffuse_map;

uniform bool use_specular_map = false;
uniform sampler2D specular_map;

uniform bool use_normal_map = false;
uniform sampler2D normal_map;

uniform bool use_shadows = false;
uniform sampler2DShadow shadowmap;

uniform sampler2D bayer;

uniform sampler2D ao;

uniform vec2 screen_size;

uniform bool selected = false;

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
	vec3 emissive;
	float shininess;
	float alpha;
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
	float attenuation = 1.0 / (pow(length(light.position - fs_in.world_pos), 2.0) + 1);
	
	vec3 diffuse =
		material.diffuse *
		diffuse_map_color *
		light.diffuse *
		light.intensity *
		max(dot(light_dir, normal), 0.0) *
		attenuation;

	vec3 specular =
		material.specular *
		specular_map_color *
		light.specular *
		light.intensity *
		pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess)
		* attenuation;

	return diffuse + specular;
}

vec3 compute_directional_light(DirectionalLight light, vec3 normal, vec3 view_dir, vec3 specular_map_color, vec3 diffuse_map_color) {
	vec3 light_dir = normalize(light.direction);
	vec3 reflect_dir = reflect(-light_dir, normal);
	
	vec3 diffuse =
			material.diffuse *
			diffuse_map_color *
			light.diffuse *
			light.intensity *
			max(dot(light_dir, normal), 0.0);

	vec3 specular =
		material.specular *
		specular_map_color *
		light.specular *
		light.intensity *
		pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);

	/* Shadow calculation. */
	float shadow = 0.0;

	if (use_shadows) {
		float bias = 0.0;

		vec3 proj_coords = fs_in.light_pos.xyz / fs_in.light_pos.w;
		proj_coords = proj_coords * 0.5 + 0.5;

		shadow = texture(shadowmap, vec3(proj_coords.xy, proj_coords.z - bias)).r;
	}

	return shadow * (diffuse + specular);
}

void main() {	
	/* This is a bit of a hack to get the texture coordinates
	 * of a full-screen quad. It won't work if the final scene quad isn't
	 * being drawn to the entire screen, as gl_FragCoord is
	 * relative to the window, not the quad vertices. */
	vec2 screen_texcoord = gl_FragCoord.xy / screen_size;

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

	/* Really SSAO should be for deferred lighting... */
	vec4 ambient_occlusion = texture(ao, screen_texcoord);

	vec3 lighting_result =
			diffuse_map_color.rgb *
			world.ambient_intensity *
			world.ambient *
			material.ambient;

	lighting_result *= ambient_occlusion.rgb;

	lighting_result += material.emissive;

	for (uint i = 0; i < point_light_count; i++) {
		lighting_result += compute_point_light(point_lights[i], n, view_dir,
			specular_map_color.rgb, diffuse_map_color.rgb);
	}
	
	for (uint i = 0; i < directional_light_count; i++) {
		lighting_result += compute_directional_light(directional_lights[i], n, view_dir,
			specular_map_color.rgb, diffuse_map_color.rgb);
	}

	if (selected) {
		color = 0.5 + vec4(lighting_result, 1.0);
	} else {
		color = vec4(lighting_result, 1.0);
	}
}

#end FRAGMENT
