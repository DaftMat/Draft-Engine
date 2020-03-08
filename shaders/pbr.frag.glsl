#version 410
out vec4 fragColor;

in vec3 fragPos;
in vec3 fragNormal;
in vec2 fragTex;

struct Material {
    vec3 albedo;
    float metalness;
    float roughness;
    float ao;
};

struct PointLight {
    vec3 position;
    float constant;
    float linear;
    float quadratic;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct DirLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    float innerCutoff;
    float outerCutoff;
    float constant;
    float linear;
    float quadratic;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

#define MAX_LIGHTS 32

uniform vec3 viewPos;

uniform PointLight point_light[MAX_LIGHTS];
uniform int num_point_light;

uniform DirLight dir_light[MAX_LIGHTS];
uniform int num_dir_light;

uniform SpotLight spot_light[MAX_LIGHTS];
uniform int num_spot_light;

uniform Material material;

const float PI = 3.14159265359;

float distributionGGX(vec3 N, vec3 H, float roughness);
float geometrySchlickGGX(float NdotV, float roughness);
float geometrySmith(vec3 N, vec3 V, vec3 L, float roughness);
vec3 fresnelSchlick(float cosTheta, vec3 F0);

vec3 calcPointLight(PointLight light, vec3 N, vec3 fragPos, vec3 V);
vec3 calcDirLight(DirLight light, vec3 N, vec3 V);
vec3 calcSpotLight(SpotLight light, vec3 N, vec3 fragPos, vec3 V);

void main() {
    vec3 N = normalize(fragNormal);
    vec3 V = normalize(viewPos - fragPos);
    vec3 Lo = vec3(0.0);

    //treat all lights
    for (int i = 0 ; i < num_point_light ; ++i) {
        Lo += calcPointLight(point_light[i], N, fragPos, V);
    }

    for (int i = 0 ; i < num_dir_light ; ++i) {
        Lo += calcDirLight(dir_light[i], N, V);
    }

    for (int i = 0 ; i < num_spot_light ; ++i) {
        Lo += calcSpotLight(spot_light[i], N, fragPos, V);
    }

    vec3 ambient = vec3(0.03) * material.albedo * material.ao;
    vec3 color = ambient + Lo;
    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0/3.3));
    fragColor = vec4(color, 1.0);
}

float distributionGGX(vec3 N, vec3 H, float roughness) {
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;

    float b = (NdotH2 * (a2 - 1.0) + 1.0);
    b = PI * b * b;
    return a2 / max(b, 0.001);
}

float geometrySchlickGGX(float NdotV, float roughness) {
    float r = roughness + 1.0;
    float k = (r * r) / 8.0;
    float a = NdotV * (1.0 - k) + k;
    return NdotV / a;
}

float geometrySmith(vec3 N, vec3 V, vec3 L, float roughness) {
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = geometrySchlickGGX(NdotV, roughness);
    float ggx1 = geometrySchlickGGX(NdotL, roughness);
    return ggx1 * ggx2;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0) {
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

vec3 F0 = mix(vec3(0.04), material.albedo, material.metalness);

vec3 calcPointLight(PointLight light, vec3 N, vec3 fragPos, vec3 V) {
    vec3 L = normalize(light.position - fragPos);
    vec3 H = normalize(V + L);
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    vec3 radiance = light.specular * attenuation;

    float NDF = distributionGGX(N, H, material.roughness);
    float G = geometrySmith(N, V, L, material.roughness);
    vec3 F = fresnelSchlick(clamp(dot(H, V), 0.0, 1.0), F0);

    vec3 a = F * NDF * G;
    float b = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001;
    vec3 specular = a / b;

    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - material.metalness;
    float NdotL = max(dot(N, L), 0.0);

    return (kD * material.albedo / PI + specular) * radiance * NdotL;
}

vec3 calcDirLight(DirLight light, vec3 N, vec3 V) {
    vec3 L = normalize(-light.direction);
    vec3 H = normalize(V + L);
    vec3 radiance = light.specular;

    float NDF = distributionGGX(N, H, material.roughness);
    float G = geometrySmith(N, V, L, material.roughness);
    vec3 F = fresnelSchlick(clamp(dot(H, V), 0.0, 1.0), F0);

    vec3 a = NDF * G * F;
    float b = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001;
    vec3 specular = a / b;

    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - material.metalness;
    float NdotL = max(dot(N, L), 0.0);

    return (kD * material.albedo / PI + specular) * radiance * NdotL;
}

vec3 calcSpotLight(SpotLight light, vec3 N, vec3 fragPos, vec3 V) {
    vec3 L = normalize(light.position - fragPos);
    vec3 H = normalize(V + L);
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    float theta = dot(L, normalize(-light.direction));
    float epsilon = light.innerCutoff - light.outerCutoff;
    float intensity = clamp((theta - light.outerCutoff) / epsilon, 0.0, 1.0);
    vec3 radiance = light.specular * attenuation * intensity;

    float NDF = distributionGGX(N, H, material.roughness);
    float G = geometrySmith(N, V, L, material.roughness);
    vec3 F = fresnelSchlick(clamp(dot(H, V), 0.0, 1.0), F0);

    vec3 a = NDF * G * F;
    float b = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001;
    vec3 specular = a / b;

    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - material.metalness;
    float NdotL = max(dot(N, L), 0.0);

    return (kD * material.albedo / PI + specular) * radiance * NdotL;
}