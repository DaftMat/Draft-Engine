/**
 * Created by mathis on 11/02/2020.
 */

#include "IcoSphere.hpp"

void IcoSphere::setupIcosahedron() {
    const float H_ANGLE = glm::pi<float>() / 180 * 72;
    const float V_ANGLE = glm::atan(.5f);

    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    float z, xy;
    float h_angle1 = -glm::pi<float>() / 2.f - H_ANGLE / 2.f;
    float h_angle2 = -glm::pi<float>() / 2.f;

    // first vertex
    Vertex vertex;
    vertex.Position = vertex.Normal = glm::vec3(0.f, 1.f, 0.f);
    vertex.TexCoords = glm::vec2(0.f, 0.f);
    vertices.push_back(vertex);

    // 10 "middle" vertices
    for (int i = 1 ; i <= 5 ; ++i) {
        z = glm::sin(V_ANGLE);
        xy = glm::cos(V_ANGLE);

        vertex.Position = vertex.Normal = glm::vec3(xy * glm::cos(h_angle1), z, xy * glm::sin(h_angle1));
        vertices.push_back(vertex);

        vertex.Position = vertex.Normal = glm::vec3(xy * glm::cos(h_angle2), -z, xy * glm::sin(h_angle2));
        vertices.push_back(vertex);

        h_angle1 += H_ANGLE;
        h_angle2 += H_ANGLE;
    }

    // last vertex
    vertex.Position = vertex.Normal = glm::vec3(0.f, -1.f, 0.f);
    vertices.push_back(vertex);

    // indices
    for (GLuint i = 1 ; i <= 9 ; i+=2) {
        // Top
        indices.push_back(0);
        indices.push_back(mod1(i+2, 10));
        indices.push_back(i);
        // Middle up
        indices.push_back(i);
        indices.push_back(mod1(i+2, 10));
        indices.push_back(mod1(i+1, 10));
        // Middle down
        indices.push_back(mod1(i+2, 10));
        indices.push_back(mod1(i+3, 10));
        indices.push_back(mod1(i+1, 10));
        // Bottom
        indices.push_back(mod1(i+1, 10));
        indices.push_back(mod1(i+3, 10));
        indices.push_back(11);
    }

    m_meshes.clear();
    m_meshes.emplace_back(vertices, indices);
}

void IcoSphere::setupSubdivisions() {
    std::vector<Vertex> tmpVertices = m_meshes[0].vertices();
    std::vector<GLuint> tmpIndices = m_meshes[0].indices();
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    GLuint v1_ind, v2_ind, v3_ind;
    Vertex new_v1, new_v2, new_v3;
    GLuint index = 0;

    for (GLuint i = 0 ; i < tmpIndices.size() ; i += 3) {
        v1_ind = tmpIndices[i];
        v2_ind = tmpIndices[i+1];
        v3_ind = tmpIndices[i+2];

        /// 3 new vertices
        new_v1.Position = new_v1.Normal = computeHalfVertex(tmpVertices[v1_ind].Position, tmpVertices[v2_ind].Position);
        new_v2.Position = new_v2.Normal = computeHalfVertex(tmpVertices[v2_ind].Position, tmpVertices[v3_ind].Position);
        new_v3.Position = new_v3.Normal = computeHalfVertex(tmpVertices[v1_ind].Position, tmpVertices[v3_ind].Position);

        /// Add all vertices to the fresh vertices list
        // 1st triangle
        vertices.push_back(tmpVertices[v1_ind]);
        vertices.push_back(new_v1);
        vertices.push_back(new_v3);
        // 2nd triangle
        vertices.push_back(new_v1);
        vertices.push_back(tmpVertices[v2_ind]);
        vertices.push_back(new_v2);
        // 3rd triangle
        vertices.push_back(new_v1);
        vertices.push_back(new_v2);
        vertices.push_back(new_v3);
        // 4th triangle
        vertices.push_back(new_v3);
        vertices.push_back(new_v2);
        vertices.push_back(tmpVertices[v3_ind]);

        /// 4 new triangles
        for (int j = 0 ; j < 12 ; ++j)
            indices.push_back(index + j);
        index += 12;
    }

    m_meshes.clear();
    m_meshes.emplace_back(vertices, indices);
}

GLuint mod1(GLuint x, GLuint m) {
    return m == x ? x : x%m;
}