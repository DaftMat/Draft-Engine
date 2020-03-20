//
//Created by mathis on 20/03/2020.
//

#ifndef DAFT_ENGINE_RAYTRACER_HPP
#define DAFT_ENGINE_RAYTRACER_HPP


#include <memory>
#include <src/Engine/Geometry/Model.hpp>
#include <src/Engine/Geometry/Lights/Light.hpp>
#include <src/Engine/Physics/Ray.hpp>
#include <src/Engine/Geometry/Lights/PointLight.hpp>

class Raytracer {

public:
    /** Constructor.
     *
     * @param width - screen width.
     * @param height - screen height.
     */
    Raytracer(int width, int height) { reset(width, height); }

    ///uncopyable
    Raytracer(const Raytracer &) = delete;
    Raytracer(Raytracer &&) = delete;
    Raytracer &operator=(const Raytracer &) = delete;
    Raytracer &operator=(Raytracer &&) = delete;

    /** Resets the raytracer.
     *
     * @param width - screen width.
     * @param height - screen height.
     */
    void reset(int width, int height);

    /** Add a Model to the raytracer's scene.
     *
     * @param model - model to add.
     */
    void addModel(Model *model);

    /** Add a Light to the raytracer's scene.
     *
     * @param light - light to add.
     */
    void addLight(Light *light);

    /** Render the scene to a png image.
     *
     * @param path - path where to create image.
     */
    void render(const std::string & path);

private:
    struct Fragment{
        glm::vec3 normal;    ///< normal from vertex normal interpolation
        glm::vec3 texNormal; ///< normal from a normal map
        glm::vec3 position;
        std::shared_ptr<Model> model;
    };

    /// Color calculation functions.
    glm::vec3 traceRay(const Ray &ray);
    glm::vec3 shade(const glm::vec3 &view, const PointLight &light, const Fragment &frag);
    glm::vec3 RDM_bsdf(float LdotH, float NdotH, float VdotH, float LdotN, float VdotN, const Fragment &frag);
    glm::vec3 RDM_bsdf_d(const Fragment &frag);
    glm::vec3 RDM_bsdf_s(float LdotH, float NdotH, float VdotH, float LdotN, float VdotN, const Fragment &frag);
    glm::vec3 RDM_smith(float LdotH, float LdotN, float VdotH, float VdotN, float alpha);
    float RDM_G1(float DdotH, float DdotN, float alpha);
    float RDM_fresnel(float LdotH, float extIOR, float intIOR);
    float RDM_Beckmann(float NdotH, float alpha);
    float RDM_chiplus(float c) { return (c > 0.f) ? 1.f : 0.f; }
    glm::vec3 calculateNormal(const Mesh::Vertex &v1, const Mesh::Vertex &v2, const Mesh::Vertex &v3);

    float m_deltax;
    float m_deltay;
    float m_aspect;

    /// Scene data
    std::shared_ptr<Model> m_models;
    std::shared_ptr<Light> m_lights;

    unsigned int m_numsamples;
};


#endif //DAFT_ENGINE_RAYTRACER_HPP
