//
// Created by daftmat on 13/02/20.
//

#ifndef DAFT_ENGINE_LIGHT_HPP
#define DAFT_ENGINE_LIGHT_HPP

//#include <opengl_stuff.h>
#include <src/Engine/Geometry/Model.hpp>

/** Type of the light.
 *  Each light has its type
 */
enum LightType {
    POINT_LIGHT, ///< A point emitting light in every directions.
    SPOT_LIGHT, ///< A point emitting light within an angle following a specific direction.
    DIR_LIGHT ///< Constant light arriving from one direction anywhere in the scene.
};

/** Settings specific to a light.
 *  Each lights type has its specifics settings.
 */
union LightParam {
    struct point_param {
        glm::vec3 position; ///< position of the point light.
        GLfloat intensity; ///< intensity of the point light.
    }pointlight;
    struct dir_param {
        glm::vec3 direction; ///< direction of the directional light.
        glm::vec3 rotations; ///< rotation applied to the directional light.
    }dirlight;
    struct spot_param {
        glm::vec3 position; ///< position of the spot light.
        glm::vec3 direction; ///< direction of the spot light.
        glm::vec3 rotations; ///< rotation applied to the spot light.
        GLfloat innerCutoff; ///< inner angle.
        GLfloat outerCutoff; ///< outer angle.
        GLfloat intensity; ///< intensity of the spot light.
    }spotlight;
};

/** Light base class.
 *  Base class of any type of Light.
 */
class Light {
public:
    /** Constructor.
     *  Base constructor of a Light.
     * @param color - color emitted by the light.
     */
    Light(const glm::vec3 &color);
    virtual ~Light() { m_model.reset(); }

    Light(const Light &) = delete;
    Light(Light &&) = delete;
    Light &operator=(const Light &) = delete;
    Light &&operator=(Light &&) = delete;

    /** Light's type getter. */
    virtual LightType getType() const = 0;
    /** Light's settings getter. */
    virtual LightParam getParams() const = 0;
    /** Light's settings setter. */
    virtual void editLight(const LightParam &params) = 0;

    /** Light's color getter. */
    const glm::vec3 & color() const { return m_color; }
    /** Light's color setter. */
    glm::vec3 & color() { return m_color; }

    /** Light's Model getter. */
    const Model& model() const { return *m_model; }
    /** Light's Model setter. */
    Model& model() { return *m_model; }

    virtual void update() {};
    void reset() { setupModel(); }

protected:
    std::unique_ptr<Model> m_model;

private:
    virtual void setupModel() {};

    glm::vec3 m_color;
};


#endif //DAFT_ENGINE_LIGHT_HPP
