#include "VAO.h"

namespace GDSA::Render
{
VAO::VAO(bool interleaved)
{
    glGenVertexArrays(1, &_vao);
    glBindVertexArray(_vao);

    // VBOs
    _vbos.resize(interleaved ? 1 : VBO_slots::NUM_VBOS);
    glGenBuffers(static_cast<GLsizei>(_vbos.size()), _vbos.data());

    if(!interleaved)
    {
        for(int vbo = 0; vbo < VBO_slots::NUM_VBOS; ++vbo)
        {
            this->defineNonInterleaveVBO(_vbos[vbo], sizeof(glm::vec4), GL_FLOAT, static_cast<uint8_t>(vbo));
        }
    }
    else
    {
        this->defineInterleavedVBO(_vbos[0]);
    }

    // IBOs
    _ibos.resize(IBO_slots::NUM_IBOS);
    glGenBuffers(static_cast<GLsizei>(_ibos.size()), _ibos.data());
}

VAO::~VAO()
{
    glDeleteBuffers(static_cast<GLsizei>(_vbos.size()), _vbos.data());
    glDeleteBuffers(static_cast<GLsizei>(_ibos.size()), _ibos.data());
    glDeleteVertexArrays(1, &_vao);
}

void VAO::drawObject(IBO_slots ibo, GLuint openGLPrimitive, GLuint numIndices) const
{
    glBindVertexArray(_vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibos[ibo]);
    glDrawElements(openGLPrimitive, numIndices, GL_UNSIGNED_INT, nullptr);
}

void VAO::drawObject(IBO_slots ibo, GLuint openGLPrimitive, GLuint numIndices, GLuint numInstances) const
{
    glBindVertexArray(_vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibos[ibo]);
    glDrawElementsInstanced(openGLPrimitive, numIndices, GL_UNSIGNED_INT, nullptr, numInstances);
}

void VAO::setVBOData(const std::vector<Vertex>& vertices, GLuint changeFrequency) const
{
    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER, _vbos[0]);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), changeFrequency);
}

void VAO::setIBOData(IBO_slots ibo, const std::vector<GLuint>& indices, GLuint changeFrequency) const
{
    glBindVertexArray(_vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibos[ibo]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), changeFrequency);
}

void VAO::defineNonInterleaveVBO(GLuint vboId, size_t structSize, GLuint elementType, uint8_t slot) const
{
    glBindBuffer(GL_ARRAY_BUFFER, vboId);
    glBufferData(GL_ARRAY_BUFFER, structSize, nullptr, GL_STATIC_DRAW);
    glVertexAttribPointer(slot, static_cast<GLsizei>(structSize / sizeof(elementType)), elementType, GL_FALSE, static_cast<GLsizei>(structSize), ((GLubyte*)nullptr));
    glEnableVertexAttribArray(slot);
}

void VAO::defineInterleavedVBO(GLuint vboId) const
{
    glBindBuffer(GL_ARRAY_BUFFER, vboId);
    constexpr GLsizei structSize = sizeof(Vertex);

    glEnableVertexAttribArray(VBO_slots::VBO_POSITION);
    glVertexAttribPointer(VBO_slots::VBO_POSITION, static_cast<GLsizei>(sizeof(glm::vec3) / sizeof(GL_FLOAT)), GL_FLOAT, GL_FALSE, structSize, (GLubyte*)offsetof(Vertex, _position));

    glEnableVertexAttribArray(VBO_slots::VBO_NORMAL);
    glVertexAttribPointer(VBO_slots::VBO_NORMAL, static_cast<GLsizei>(sizeof(glm::vec3) / sizeof(GL_FLOAT)), GL_FLOAT, GL_FALSE, structSize, (GLubyte*)offsetof(Vertex, _normal));

    glEnableVertexAttribArray(VBO_slots::VBO_TEXT_COORD);
    glVertexAttribPointer(VBO_slots::VBO_TEXT_COORD, static_cast<GLsizei>(sizeof(glm::vec2) / sizeof(GL_FLOAT)), GL_FLOAT, GL_FALSE, structSize, (GLubyte*)offsetof(Vertex, _textCoord));
}
}    // namespace GDSA::Render
