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
#include <src/Engine/Cameras/camera.h>

/** Ray-tracing based renderer.
 * IS NOT an OpenGL renderer.
 * WILL NOT render anything on an OpenGL context.
 */
class Raytracer {

public:
    /** Constructor.
     *
     * @param width - screen width.
     * @param height - screen height.
     */
    Raytracer(int width, int height, const glm::vec3 skyColor = glm::vec3(0.f), unsigned int numsamples = 4)
    : m_skyColor { skyColor }, m_numsamples { numsamples } { reset(width, height); }

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
    void addModel(const Model &model);

    /** Add a Light to the raytracer's scene.
     *
     * @param light - light to add.
     */
    void addLight(const Light &light);

    /** Number of samples per pixel setter.
     *
     * @param s - new number of samples.
     */
    void setSamples(unsigned int samples) { m_numsamples = samples; }

    /** Render the scene to a png image.
     *
     * @param path - path where to create image.
     */
    void render(const std::string & path, const glm::mat4 & projection, const glm::mat4 & view);

private:
    const float ACNE_EPS = 1.e-2f;

    struct Fragment {
        glm::vec3 normal;
        glm::vec3 position;
        Material material;
    };

    struct SimpleMesh {
        std::vector<Mesh::Vertex> vertices;
        std::vector<GLuint> indices;
        glm::mat4 model;
        Material materal;
    };

    struct SimpleLight {
        Light::LightType type;
        Light::LightParam param;
        glm::vec3 color;
    };

    /// Color calculation functions.
    glm::vec3 traceRay(const Ray &ray, float reflcoef = 1.f);
    glm::vec3 shade(const glm::vec3 &view, const SimpleLight &light, const glm::vec3 &L, const Fragment &frag);
    glm::vec3 RDM_bsdf(float LdotH, float NdotH, float VdotH, float LdotN, float VdotN, const Fragment &frag);
    glm::vec3 RDM_bsdf_d(const Fragment &frag);
    glm::vec3 RDM_bsdf_s(float LdotH, float NdotH, float VdotH, float LdotN, float VdotN, const Fragment &frag);
    float RDM_smith(float LdotH, float LdotN, float VdotH, float VdotN, float alpha);
    float RDM_G1(float DdotH, float DdotN, float alpha);
    float RDM_fresnel(float LdotH, float extIOR, float intIOR);
    float RDM_Beckmann(float NdotH, float alpha);
    float RDM_chiplus(float c) { return (c > 0.f) ? 1.f : 0.f; }
    bool intersectScene(const Ray &ray, Fragment &frag, float dist = 10000000.f);
    /** Calculates interpolated normal. */
    glm::vec3 calculateNormal(const Mesh::Vertex &v1, const Mesh::Vertex &v2, const Mesh::Vertex &v3);
    void calculateLight(const SimpleLight &light, const Fragment &frag, glm::vec3 &L, float &factor, float &dist);
    glm::vec3 skyLighting(const Fragment &frag, const Ray &ray);

    float m_screenWidth;
    float m_screenHeight;

    /// Scene data
    std::vector<SimpleMesh> m_meshes;
    std::vector<SimpleLight> m_lights;

    glm::vec3 m_skyColor;

    unsigned int m_numsamples;
};


#endif //DAFT_ENGINE_RAYTRACER_HPP
