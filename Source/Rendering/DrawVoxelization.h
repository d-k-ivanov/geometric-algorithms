#pragma once

#include "Model3D.h"

namespace GDSA
{
class DrawVoxelization : public Model3D
{
protected:
    int       _numVoxels;
    glm::vec3 _voxelScale;

public:
    DrawVoxelization(glm::vec3* positions, int numVoxels, const glm::vec3& voxelScale, glm::vec3* voxelColor = nullptr);
    ~DrawVoxelization() override = default;
    void draw(RenderingShader* shader, MatrixRenderInformation* matrixInformation, ApplicationState* appState, GLuint primitive) override;
};
}
