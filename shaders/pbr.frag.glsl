#version 410
out vec4 fragColor;

in vec3 fragPos;
in vec3 fragNormal;
in vec2 fragTex;

struct Material {
    vec3 albedo;
    vec3 specular;
    float roughness;
    float ao;
    float ior;
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

uniform vec3 viewPos;

uniform PointLight point_light[MAX_LIGHTS];
uniform int num_point_light;

uniform DirLight dir_light[MAX_LIGHTS];
uniform int num_dir_light;

uniform SpotLight spot_light[MAX_LIGHTS];
uniform int num_spot_light;

uniform Material material;

const float PI = 3.14159265359;

float RDM_chiplus(float c) { return (c > 0.f) ? 1.f : 0.f; }
float RDM_Beckmann(float NdotH, float alpha);
float RDM_Fresnel(float LdotH, float extIOR, float intIOR);
float RDM_G1(float DdotH, float DdotN, float alpha);
float RDM_Smith(float LdotH, float LdotN, float VdotH, float VdotN, float alpha);
vec3 RDM_bsdf_s(float LdotH, float NdotH, float VdotH, float LdotN, float VdotN);
vec3 RDM_bsdf_d();
vec3 RDM_bsdf(float LdotH, float NdotH, float VdotH, float LdotN, float VdotN);
vec3 shade(PointLight light, vec3 N, vec3 V, vec3 fragPos);
vec3 shade(DirLight light, vec3 N, vec3 V);
vec3 shade(SpotLight light, vec3 N, vec3 V, vec3 fragPos);

void main() {
    vec3 N = normalize(fragNormal);
    vec3 V = normalize(viewPos - fragPos);
    vec3 Lo = vec3(0.0);

    //treat all lights
    for (int i = 0 ; i < num_point_light ; ++i) {
        Lo += shade(point_light[i], N, V, fragPos);
    }

    for (int i = 0 ; i < num_dir_light ; ++i) {
        Lo += shade(dir_light[i], N, V);
    }

    for (int i = 0 ; i < num_spot_light ; ++i) {
        Lo += shade(spot_light[i], N, V, fragPos);
    }

    vec3 ambient = vec3(0.03) * material.albedo * material.ao;
    vec3 color = ambient + Lo;
    color = color / (color + vec3(1.0));
    color = pow(color, vec3(0.5f));
    fragColor = vec4(color, 1.0);
}

float RDM_Beckmann(float NdotH, float alpha) {
    float cosS = NdotH * NdotH;
    float tanS = (1.f - cosS) * 1.f / cosS;
    float alphaS = alpha * alpha;
    float ret = RDM_chiplus(NdotH) * (exp(-tanS / alphaS) * (1.f / (PI * alphaS * (cosS * cosS))));
    return ret;
}

float RDM_Fresnel(float LdotH, float extIOR, float intIOR) {
    float sinSt = ((extIOR * extIOR) / (intIOR * intIOR)) * (1.f - LdotH * LdotH);
    if (sinSt > 1.f)    return 1.f;
    float cosT = sqrt(1.f - sinSt);
    float s1 = (extIOR * LdotH - intIOR * cosT);
    float s2 = (extIOR * LdotH + intIOR * cosT);
    float p1 = (extIOR * cosT - intIOR * LdotH);
    float p2 = (extIOR * cosT + intIOR * LdotH);
    float Rs = (s1 * s1) / (s2 * s2);
    float Rp = (p1 * p1) / (p2 * p2);
    return 0.5f * (Rs + Rp);
}

float RDM_G1(float DdotH, float DdotN, float alpha) {
    float tanTheta = sqrt(1.f-(DdotN*DdotN))*(1.0f/DdotN);
    float b = 1.f/(alpha * tanTheta);
    float k = DdotH * (1.f/DdotN);
    float G1 = RDM_chiplus(k);
    if (b < 1.6f){
        G1 *= (3.535f*b + 2.181f*b*b)/(1.0f+2.276f*b+2.577f*b*b);
    }
    return G1;
}

float RDM_Smith(float LdotH, float LdotN, float VdotH, float VdotN, float alpha) {
    return RDM_G1(LdotH, LdotN, alpha) * RDM_G1(VdotH, VdotN, alpha);
}

vec3 RDM_bsdf_s(float LdotH, float NdotH, float VdotH, float LdotN, float VdotN) {
    float D = RDM_Beckmann(NdotH, material.roughness);
    float F = RDM_Fresnel(LdotH, 1.f, material.ior);
    float G = RDM_Smith(LdotH, LdotN, VdotH, VdotN, material.roughness);
    return material.specular * (D*F*G)/(4.f*LdotN*VdotN);
}

vec3 RDM_bsdf_d() {
    return (material.albedo * (1.f / PI));
}

vec3 RDM_bsdf(float LdotH, float NdotH, float VdotH, float LdotN, float VdotN) {
    return RDM_bsdf_s(LdotH, NdotH, VdotH, LdotN, VdotN) + RDM_bsdf_d();
}

vec3 shade(PointLight light, vec3 N, vec3 V, vec3 fragPos) {
    vec3 L = normalize(light.position - fragPos);
    vec3 H = normalize(V + L);
    float distance = length(light.position - fragPos);
    float attenuation = light.intensity / (distance * distance);

    float LdotH = dot(L, H);
    float NdotH = dot(N, H);
    float VdotH = dot(V, H);
    float LdotN = dot(L, N);
    float VdotN = dot(V, N);

    return light.color * RDM_bsdf(LdotH, NdotH, VdotH, LdotN, VdotN) * attenuation * max(LdotN, 0.f);
}

vec3 shade(DirLight light, vec3 N, vec3 V) {
    vec3 L = normalize(-light.direction);
    vec3 H = normalize(V + L);

    float LdotH = dot(L, H);
    float NdotH = dot(N, H);
    float VdotH = dot(V, H);
    float LdotN = dot(L, N);
    float VdotN = dot(V, N);

    return light.color * RDM_bsdf(LdotH, NdotH, VdotH, LdotN, VdotN) * max(LdotN, 0.f);
}

vec3 shade(SpotLight light, vec3 N, vec3 V, vec3 fragPos) {
    vec3 L = normalize(light.position - fragPos);
    vec3 H = normalize(V + L);
    float distance = length(light.position - fragPos);
    float attenuation = light.intensity / (distance * distance);
    float theta = dot(L, normalize(-light.direction));
    float epsilon = light.innerCutoff - light.outerCutoff;
    float intensity = clamp((theta - light.outerCutoff) / epsilon, 0.0, 1.0);

    float LdotH = dot(L, H);
    float NdotH = dot(N, H);
    float VdotH = dot(V, H);
    float LdotN = dot(L, N);
    float VdotN = dot(V, N);

    return light.color * RDM_bsdf(LdotH, NdotH, VdotH, LdotN, VdotN) * attenuation * intensity * max(LdotN, 0.f);
}