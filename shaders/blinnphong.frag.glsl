#version 410
out vec4 fragColor;

struct Material {
    vec3 specular;
    vec3 albedo;
    float shininess;
    bool isParticle;
};

struct PointLight {
    vec3 position;
    float intensity;
    vec3 color;
};

struct DirLight {
    vec3 direction;
    vec3 color;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    float innerCutoff;
    float outerCutoff;
    float intensity;
    vec3 color;
};

#define MAX_LIGHTS 32

in vec3 fragPos;
in vec3 fragNormal;
in vec2 fragTex;

uniform vec3 viewPos;

uniform PointLight point_light[MAX_LIGHTS];
uniform int num_point_light;

uniform DirLight dir_light[MAX_LIGHTS];
uniform int num_dir_light;

uniform SpotLight spot_light[MAX_LIGHTS];
uniform int num_spot_light;

uniform Material material;

vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 calcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
float alpha();

void main() {
    vec3 normal = normalize(fragNormal);
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 resultColor = vec3(0.0);

    for (int i = 0 ; i < num_point_light ; ++i) {
        resultColor += calcPointLight(point_light[i], normal, fragPos, viewDir);
    }

    for (int i = 0 ; i < num_dir_light ; ++i) {
        resultColor += calcDirLight(dir_light[i], normal, viewDir);
    }

    for (int i = 0 ; i < num_spot_light ; ++i) {
        resultColor += calcSpotLight(spot_light[i], normal, fragPos, viewDir);
    }

    vec3 ambient = vec3(0.03) * material.albedo;
    vec3 color = ambient + resultColor;
    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0/2.2));
    fragColor = vec4(color, alpha());
}

vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
    float distance = length(light.position - fragPos);
    float attenuation = light.intensity / (distance * distance);
    vec3 diffuse = light.color * diff * vec3(material.albedo) * attenuation;
    vec3 specular = light.color * spec * vec3(material.specular) * attenuation;
    return diffuse + specular;
}

vec3 calcDirLight(DirLight light, vec3 normal, vec3 viewDir){
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
    vec3 diffuse = light.color * diff * vec3(material.albedo);
    vec3 specular = light.color * spec * vec3(material.specular);
    return diffuse + specular;
}

vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
    float distance = length(light.position - fragPos);
    float attenuation = light.intensity / (distance * distance);
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.innerCutoff - light.outerCutoff;
    float intensity = clamp((theta - light.outerCutoff) / epsilon, 0.0, 1.0);;
    vec3 diffuse = light.color * diff * vec3(material.albedo);
    vec3 specular = light.color * spec * vec3(material.specular);
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return diffuse + specular;
}

float alpha() {
    if (!material.isParticle)
    return 1.0;
    float dist = length(fragTex - vec2(0.5));
    return 1.0 - (dist / 0.5);
}