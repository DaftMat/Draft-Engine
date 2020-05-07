#version 410
out vec4 fragColor;

struct Material {
    vec3 specular;
    vec3 albedo;
    float shininess;
};

in vec3 fragPos;
in vec3 fragNormal;
in vec2 fragTex;

uniform vec3 particlePosition;
uniform float particleRadius;

uniform Material material;

void main() {
    float dist = length(fragTex - vec2(0.5));
    float alpha = dist / 0.5;
    fragColor = vec4(material.albedo, 1.0-alpha);
}
