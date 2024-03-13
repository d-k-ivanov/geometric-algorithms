#pragma once

// #include "DrawVoxelization.h"
#include "PointCloud3d.h"
#include "Triangle3d.h"
#include "Vect2d.h"
#include "Vect3d.h"

#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>

#define BINARY_EXTENSION ".bin"

class TriangleModel
{
private:
    std::vector<Vect3d>   _vertices;
    std::vector<Vect3d>   _normals;
    std::vector<Vect2d>   _textCoordinates;
    std::vector<unsigned> _indices;

protected:
    void loadModelBinaryFile(const std::string& path);
    void processMesh(aiMesh* mesh, const aiScene* scene, const std::string& folder);
    void processNode(aiNode* node, const aiScene* scene, const std::string& folder);
    void writeBinaryFile(const std::string& path);

public:
    TriangleModel(const std::string& pathfile);
    TriangleModel(const TriangleModel& orig) = delete;
    virtual ~TriangleModel();

    PointCloud3d            getCloud();
    Triangle3d              getFace(unsigned index);
    std::vector<Triangle3d> getFaces();
    std::vector<Vect3d>*    getVertices() { return &_vertices; }
    std::vector<Vect3d>*    getNormals() { return &_normals; }
    std::vector<Vect2d>*    getTextureCoordinates() { return &_textCoordinates; }
    std::vector<unsigned>*  getIndices() { return &_indices; }
    size_t                  numTriangles();
    // AlgGeom::DrawVoxelization* voxelize();
};
