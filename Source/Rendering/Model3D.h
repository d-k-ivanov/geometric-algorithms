#pragma once

#include "ApplicationState.h"
#include "RenderingShader.h"
#include "Texture.h"
#include "TextureList.h"
#include "VAO.h"

#include "Geometry/AABB.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>

#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>

#include <string>
#include <unordered_set>
#include <vector>

#define BINARY_EXTENSION ".bin"
#define FBX_EXTENSION ".fbx"
#define GLTF_EXTENSION ".gltf"
#define OBJ_EXTENSION ".obj"

namespace GDSA::Render
{
class Model3D
{
    friend class GUI;

protected:
    struct Material
    {
        glm::vec4 _kdColor;
        glm::vec3 _ksColor;
        float     _metallic, _roughnessK;
        Texture*  _kadTexture;
        bool      _useUniformColor;
        glm::vec3 _pointColor;
        glm::vec3 _lineColor;

        Material()
            : _kdColor(1.00, 0.81, 0.29, 1.0f)
            , _ksColor(.5f)
            , _metallic(.7f)
            , _roughnessK(.3f)
            , _kadTexture(nullptr)
            , _useUniformColor(true)
            , _pointColor(.0f)
            , _lineColor(.0f)
        {
        }
    };

public:
    struct Component
    {
        bool        _enabled;
        std::string _name;

        std::vector<VAO::Vertex> _vertices;
        std::vector<GLuint>      _indices[VAO::IBO_slots::NUM_IBOS];
        VAO*                     _vao;

        Material _material;

        float _lineWidth, _pointSize;
        bool  _activeRendering[VAO::IBO_slots::NUM_IBOS];

        Component(VAO* vao = nullptr)
        {
            _enabled   = true;
            _vao       = vao;
            _pointSize = 3.0f;
            _lineWidth = 1.0f;
            for(int i = 0; i < VAO::IBO_slots::NUM_IBOS; ++i)
            {
                _activeRendering[i] = true;
            }
        }
        ~Component()
        {
            delete _vao;
            _vao = nullptr;
        }

        void completeTopology();
        void generateWireframe();
        void generatePointCloud();
    };

public:
    struct MatrixRenderInformation
    {
        enum MatrixType
        {
            MODEL,
            VIEW,
            VIEW_PROJECTION
        };

        glm::mat4              _matrix[MatrixType::VIEW_PROJECTION + 1];
        std::vector<glm::mat4> _heapMatrices[MatrixType::VIEW_PROJECTION + 1];

        MatrixRenderInformation();
        glm::mat4 multiplyMatrix(MatrixType tMatrix, const glm::mat4& matrix)
        {
            this->saveMatrix(tMatrix);
            return _matrix[tMatrix] *= matrix;
        }
        void saveMatrix(MatrixType tMatrix) { _heapMatrices[tMatrix].push_back(_matrix[tMatrix]); }
        void setMatrix(MatrixType tMatrix, const glm::mat4& matrix)
        {
            this->saveMatrix(tMatrix);
            _matrix[tMatrix] = matrix;
        }
        void undoMatrix(MatrixType type);
    };

protected:
    static std::string                     CHECKER_PATTERN_PATH;
    static std::unordered_set<std::string> USED_NAMES;

protected:
    std::vector<std::unique_ptr<Component>> _components;
    glm::mat4                               _modelMatrix;
    std::string                             _name;

protected:
    void       buildVao(Component* component) const;
    void       loadModelBinaryFile(const std::string& path);
    void       writeBinaryFile(const std::string& path) const;
    Component* getVoxel();

public:
    Model3D();
    virtual ~Model3D() = default;

    bool         belongsModel(const Component* component) const;
    virtual void draw(RenderingShader* shader, MatrixRenderInformation* matrixInformation, ApplicationState* appState, GLuint primitive);
    glm::mat4    getModelMatrix() const { return _modelMatrix; }
    std::string  getName() { return _name; }
    Model3D*     moveGeometryToOrigin(const glm::mat4& origMatrix = glm::mat4(1.0f), float maxScale = FLT_MAX);
    Model3D*     overrideModelName();
    Model3D*     setModelMatrix(const glm::mat4& modelMatrix)
    {
        _modelMatrix = modelMatrix;
        return this;
    }
    Model3D* setLineColor(const glm::vec3& color);
    Model3D* setPointColor(const glm::vec3& color);
    Model3D* setTriangleColor(const glm::vec4& color);
    Model3D* setLineWidth(float width);
    Model3D* setPointSize(float size);
    Model3D* setTopologyVisibility(VAO::IBO_slots topology, bool visible);
};
}    // namespace GDSA::Render
