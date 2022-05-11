#version 430 core

struct Material {
	sampler2D texture_diffuse0;
	sampler2D texture_specular0;
	float shininess;
};

struct DirLight {
	vec3 direction;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight {
	vec3 position;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	
	vec3 attenuation;
};

struct SpotLight {
	vec3 position;
	vec3 direction;
	float cutoff;
	float outerCutoff;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	
	vec3 attenuation;
};

vec3 CalcDirLight(Material m, DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(Material m, PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(Material m, SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 FragColor;

uniform vec3 viewPos;

uniform Material material;
uniform DirLight dirLight;
#define NR_POINT_LIGHTS 4
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLight;

void main()
{
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);
	
	vec3 result = vec3(0.0f);
	
	// Directional lighting
	result += CalcDirLight(material, dirLight, norm, viewDir);
	
	// Point lighting
	for (int i = 0; i < NR_POINT_LIGHTS; i++){
		result += CalcPointLight(material, pointLights[i], norm, FragPos, viewDir);
	}
	
	// Spot lighting
	result += CalcSpotLight(material, spotLight, norm, FragPos, viewDir);
	
	FragColor = vec4(result, 1.0);
}

vec3 CalcDirLight(Material m, DirLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(-light.direction);
	vec3 norm = normalize(normal);
	
	// ambient
	vec3 ambient = light.ambient * (texture(m.texture_diffuse0, TexCoords)).rgb;
	
	// diffuse
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * light.diffuse * (texture(m.texture_diffuse0, TexCoords)).rgb;
	
	// specular
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = spec * light.specular * (texture(m.texture_specular0, TexCoords)).rgb;
	
	vec3 result =  (ambient + diffuse + specular);
	
	return result;
}

vec3 CalcPointLight(Material m, PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fragPos);
	vec3 norm = normalize(normal);
	
	// ambient
	vec3 ambient = light.ambient * texture(m.texture_diffuse0, TexCoords).rgb;
	
	// diffuse
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * light.diffuse * texture(m.texture_diffuse0, TexCoords).rgb;
	
	// specular
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = spec * light.specular * texture(m.texture_specular0, TexCoords).rgb;
	
	// attenuation
	float d = length(light.position - FragPos);
	float attenuation = 1.0 / dot(light.attenuation, vec3(1.0, d, d * d));
	
	return (attenuation * (ambient + diffuse + specular));
}

vec3 CalcSpotLight(Material m, SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(light.position - FragPos);
	float costheta = dot(lightDir, normalize(-light.direction));
	
	// ambient
	vec3 ambient = light.ambient * vec3(texture(m.texture_diffuse0, TexCoords));
	
	// attenuation
	float d = length(light.position - FragPos);
	float attenuation = 1.0/(dot(light.attenuation, vec3(1.0, d, d * d)));
	
	vec3 result;
	
	if (costheta > light.outerCutoff){
		// smooth spotlight
		float epsilon = light.cutoff - light.outerCutoff;
		float intensity = clamp((costheta - light.outerCutoff) / epsilon, 0.0, 1.0);
		
		// diffuse
		float diff = max(dot(norm, lightDir), 0.0);
		vec3 diffuse = light.diffuse * diff * intensity * vec3(texture(m.texture_diffuse0, TexCoords));
		
		// specular
		vec3 reflectDir = reflect(-lightDir, norm);
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
		vec3 specular = light.specular * spec * intensity * vec3(texture(m.texture_specular0, TexCoords));
		
		result = attenuation * (ambient + diffuse + specular);
	}
	else {
		result = attenuation * ambient;
	}
	
	return result;
}
