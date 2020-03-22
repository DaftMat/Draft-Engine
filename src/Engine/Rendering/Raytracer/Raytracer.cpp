/**
 * Created by mathis on 20/03/2020.
 */

#include <src/Utils/image.h>
#include "Raytracer.hpp"

void Raytracer::reset(int width, int height) {
    m_screenWidth = float(width);
    m_screenHeight = float(height);
}

float Raytracer::RDM_Beckmann(float NdotH, float alpha) {
    float cosS = NdotH * NdotH;
    float tanS = (1.f - cosS) * 1.f / cosS;
    float alphaS = alpha * alpha;
    float D = RDM_chiplus(NdotH) * (expf(-tanS/alphaS) * (1.f / (glm::pi<float>() * alphaS * (cosS * cosS))));
    return D;
}

float Raytracer::RDM_fresnel(float LdotH, float extIOR, float intIOR) {
    float sinSt = ((extIOR * extIOR) / (intIOR * intIOR)) * (1.f - LdotH * LdotH);
    if (sinSt > 1.f)    return 1.f;
    float cosT = glm::sqrt(1.f - sinSt);
    float s1 = (extIOR * LdotH - intIOR * cosT);
    float s2 = (extIOR * LdotH + intIOR * cosT);
    float p1 = (extIOR * cosT - intIOR * LdotH);
    float p2 = (extIOR * cosT + intIOR * LdotH);
    float Rs = (s1 * s1) / (s2 * s2);
    float Rp = (p1 * p1) / (p2 * p2);
    return 0.5f * (Rs + Rp);
}

float Raytracer::RDM_G1(float DdotH, float DdotN, float alpha) {
    float tanTheta = glm::sqrt(1.f - (DdotN * DdotN)) * (1.f / DdotN);
    float b = 1.f / (alpha * tanTheta);
    float k = DdotH * (1.f / DdotN);
    float G1 = RDM_chiplus(k);
    if (b < 1.6f) {
        G1 *= (3.535f * b + 2.181f * (b * b)) / (1.0f + 2.276f * b + 2.577f * (b * b));
    }
    return G1;
}

float Raytracer::RDM_smith(float LdotH, float LdotN, float VdotH, float VdotN, float alpha) {
    return RDM_G1(LdotH, LdotN, alpha) * RDM_G1(VdotH, VdotN, alpha);
}

glm::vec3 Raytracer::RDM_bsdf_s(float LdotH, float NdotH, float VdotH, float LdotN, float VdotN,
                            const Raytracer::Fragment &frag) {
    float D = RDM_Beckmann(NdotH, frag.material.roughness());
    float F = RDM_fresnel(LdotH, 1.f, frag.material.IOR());
    float G = RDM_smith(LdotH, LdotN, VdotH, VdotN, frag.material.roughness());

    return frag.material.specular() * ((D * F * G) / (4.f * LdotN * VdotN));
}

glm::vec3 Raytracer::RDM_bsdf_d(const Raytracer::Fragment &frag) {
    return (1.f - frag.material.transparency()) * frag.material.albedo() * (1.f / glm::pi<float>());
}

glm::vec3 Raytracer::RDM_bsdf(float LdotH, float NdotH, float VdotH, float LdotN, float VdotN,
                              const Raytracer::Fragment &frag) {
    return RDM_bsdf_s(LdotH, NdotH, VdotH, LdotN, VdotN, frag) + RDM_bsdf_d(frag);
}

glm::vec3 Raytracer::calculateNormal(const Mesh::Vertex &v1, const Mesh::Vertex &v2, const Mesh::Vertex &v3) {
    return glm::normalize(v1.Normal + v2.Normal + v3.Normal);
}

void Raytracer::calculateLight(const SimpleLight &light, const Fragment &frag, glm::vec3 &L, float &factor, float &dist) {
    switch (light.type) {
        case Light::POINT_LIGHT: {
            glm::vec3 fragpoint = light.param.pointlight.position - frag.position;
            dist = glm::length(fragpoint);
            L = normalize(fragpoint);
            factor = light.param.pointlight.intensity / (dist * dist);
            break;
        } case Light::SPOT_LIGHT: {
            glm::vec3 fragspot = light.param.pointlight.position - frag.position;
            dist = glm::length(fragspot);
            L = normalize(fragspot);
            float attenuation = light.param.spotlight.intensity / (dist * dist);
            float theta = dot(L, normalize(-light.param.spotlight.direction));
            float epsilon = light.param.spotlight.innerCutoff - light.param.spotlight.outerCutoff;
            float intensity = glm::clamp((theta - light.param.spotlight.outerCutoff) / epsilon, 0.f, 1.f);
            factor = attenuation * intensity;
            break;
        } case Light::DIR_LIGHT: {
            L = glm::normalize(-light.param.dirlight.direction);
            dist = 10000.f;
            factor = 1.f;
            break;
        }
    }
}

glm::vec3 Raytracer::skyLighting(const Fragment &frag, const Ray &ray) {
    SimpleLight skyLight {};
    skyLight.type = Light::DIR_LIGHT;
    skyLight.color = m_skyColor;
    skyLight.param.dirlight.direction = -frag.normal;

    glm::vec3 L;
    float fdummy, dist;
    calculateLight(skyLight, frag, L, fdummy, dist);

    glm::vec3 color;
    Fragment dummy {};
    if (!intersectScene(Ray(frag.position + ACNE_EPS * L, L), dummy, dist)) {
        color += shade(-ray.direction(), skyLight, L, frag);
    }
    return color;
}

bool Raytracer::intersectScene(const Ray &ray, Fragment &frag, float dist) {
    bool hasIntersection = false;
    for (auto &mesh : m_meshes) {
        for (int j = 0 ; j < mesh.indices.size() ; j += 3) {
            glm::vec3 k {
                mesh.indices[j],
                mesh.indices[j + 1],
                mesh.indices[j + 2]
            };
            std::vector<Mesh::Vertex> v;
            for (int vi = 0 ; vi < 3 ; ++vi) {
                Mesh::Vertex vx = mesh.vertices[k[vi]];
                vx.Normal = glm::mat3(glm::transpose(glm::inverse(mesh.model))) * vx.Normal;
                vx.Position = glm::vec3(mesh.model * glm::vec4(vx.Position, 1.f));
                v.push_back(vx);
            }
            if (ray.intersects(v[0], v[1], v[2], dist)) {
                hasIntersection = true;
                frag.position = ray.position(dist);
                frag.normal = calculateNormal(v[0], v[1], v[2]);
                frag.material = mesh.materal;
            }
        }
    }
    return hasIntersection;
}

glm::vec3 Raytracer::shade(const glm::vec3 &view, const SimpleLight &light, const glm::vec3 &L, const Raytracer::Fragment &frag) {
    glm::vec3 H = normalize(view + L);

    float LdotH = dot(L, H);
    float NdotH = dot(frag.normal, H);
    float VdotH = dot(view, H);
    float LdotN = dot(L, frag.normal);
    float VdotN = dot(view, frag.normal);

    return light.color * RDM_bsdf(LdotH, NdotH, VdotH, LdotN, VdotN, frag) * glm::max(LdotN, 0.f);
}

glm::vec3 Raytracer::traceRay(const Ray &ray, float reflcoef) {
    glm::vec3 color(0.f);
    Fragment frag;
    Ray shadow;
    Ray reflectRay;
    Ray refractRay;

    if (intersectScene(ray, frag)) {
        color += skyLighting(frag, ray);
        for (const auto &light : m_lights) {
            glm::vec3 L;
            float attenuation, distLight;
            calculateLight(light, frag, L, attenuation, distLight);
            shadow = Ray(frag.position + ACNE_EPS * L, L);
            Fragment dummy;
            if (!intersectScene(shadow, dummy, distLight)) {
                color += glm::max(shade(-ray.direction(), light, L, frag), glm::vec3(0.f)) * attenuation;
            }
        }

        if (reflcoef > 0.1f) {
            float transparency = frag.material.transparency();
            if (transparency < 1.f) {
                glm::vec3 reflectDir = glm::normalize(glm::reflect(ray.direction(), frag.normal));
                glm::vec3 reflectOrig = frag.position + ACNE_EPS * frag.normal;
                float fr = RDM_fresnel(glm::dot(reflectDir, frag.normal), 1.f, frag.material.IOR());
                reflectRay = Ray(reflectOrig, reflectDir);
                color += (1.f - transparency) * fr * traceRay(reflectRay, reflcoef * 0.5f) * frag.material.specular() * reflcoef;
            }

            if (transparency > 0.f) {
                glm::vec3 refractDir = glm::normalize(glm::refract(ray.direction(), frag.normal, 1.f / frag.material.IOR()));
                glm::vec3 refractOrig = frag.position + ACNE_EPS * (-frag.normal);
                refractRay = Ray(refractOrig, refractDir);
                color += transparency * traceRay(refractRay, reflcoef * 0.5f) * frag.material.specular();
            }
        }

        glm::vec3 ambient = glm::vec3(0.03f) * frag.material.albedo() * frag.material.ambientOcclusion();
        color += ambient;
        color = color / (color + glm::vec3(1.0));
        color = glm::pow(color, glm::vec3(0.5f));
    } else {
        color = m_skyColor;
    }

    return color;
}

void Raytracer::render(const std::string &path, const glm::mat4 & projection, const glm::mat4 & view) {
    Image *img = initImage(m_screenWidth, m_screenHeight);

    for (int j = 0 ; j < int(m_screenHeight) ; ++j) {

        #pragma omp parallel for
        for (int i = 0 ; i < int(m_screenWidth) ; ++i) {
            glm::vec3 *px = getPixelPtr(img, i, j);
            *px = glm::vec3(0.f);
            Ray rx;
            for (float aj = -0.5f + (1.f / (2.f * float(m_numsamples))) ; aj < 0.5f ; aj += 1.f / float(m_numsamples)) {
                for (float ai = -0.5f + (1.f / (2.f * float(m_numsamples))); ai < 0.5f; ai += 1.f / float(m_numsamples)) {
                    rx = Ray(float(i) + ai, float(j) + aj, m_screenWidth, m_screenHeight, projection, view);
                    *px += traceRay(rx);
                }
            }
            *px *= 1.f / (float(m_numsamples) * float(m_numsamples));
        }
    }

    saveImage(img, path.c_str());
    freeImage(img);
}

void Raytracer::addModel(const Model &model) {
    SimpleMesh newModel {};
    newModel.materal = Material(model.material());
    newModel.model = model.model();
    for (int i = 0 ; i < model.getNumMeshes() ; ++i) {
        newModel.vertices = model.getMesh(i).getVertices();
        newModel.indices = model.getMesh(i).getIndices();
        m_meshes.push_back(newModel);
    }
}

void Raytracer::addLight(const Light &light) {
    SimpleLight newLight {};
    newLight.type = light.getType();
    newLight.param = light.getParams();
    newLight.color = glm::vec3(light.color());
    m_lights.push_back(newLight);
}