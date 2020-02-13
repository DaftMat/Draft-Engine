//
// Created by daftmat on 10/02/20.
//

#include "Model.hpp"

void Model::draw(const Shader &shader) const {
    shader.use();
    shader.setMat4("model", model());

    for (const auto & mesh : m_meshes) {
        mesh->draw();
    }
}
