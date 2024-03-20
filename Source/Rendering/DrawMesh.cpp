#include "DrawMesh.h"

#include <filesystem>
#include <iostream>

namespace GDSA::Render
{
DrawMesh::DrawMesh(Geometry::TriangleModel& triangleModel)
    : Model3D()
{
    const std::vector<Geometry::Vect3d>* vertices        = triangleModel.getVertices();
    const std::vector<Geometry::Vect3d>* normals         = triangleModel.getNormals();
    std::vector<Geometry::Vect2d>*       textCoordinates = triangleModel.getTextureCoordinates();
    std::vector<unsigned>*               indices         = triangleModel.getIndices();
    const size_t                         numFaces        = triangleModel.numTriangles();

    Component* component = new Component;

    for(size_t vertexIdx = 0; vertexIdx < vertices->size(); ++vertexIdx)
    {
        // const auto  textCoord = (textCoordinates->size() > vertexIdx - 1) ? glm::vec2 {textCoordinates->at(vertexIdx).getX(), textCoordinates->at(vertexIdx).getY()} : glm::vec2 {0.0f, 0.0f};
        VAO::Vertex vertex {glm::vec3(vertices->at(vertexIdx).getX(), vertices->at(vertexIdx).getY(), vertices->at(vertexIdx).getZ()),
                            glm::vec3(normals->at(vertexIdx).getX(), normals->at(vertexIdx).getY(), normals->at(vertexIdx).getZ())};
        // if(!textCoordinates->empty())
        // {
        //     vertex._textCoord = glm::vec2(textCoordinates->at(vertexIdx).getX(), );
        // }
        component->_vertices.push_back(vertex);
    }

    for(size_t faceIdx = 0; faceIdx < numFaces; ++faceIdx)
    {
        component->_indices[VAO::IBO_TRIANGLE].insert(component->_indices[VAO::IBO_TRIANGLE].end(),
                                                      {indices->at(faceIdx * 3 + 0), indices->at(faceIdx * 3 + 1), indices->at(faceIdx * 3 + 2), RESTART_PRIMITIVE_INDEX});
    }

    component->completeTopology();
    this->buildVao(component);
    this->_components.push_back(std::unique_ptr<Component>(component));
}

DrawMesh* DrawMesh::loadModelOBJ(const std::string& path)
{
    const std::string binaryFile = path.substr(0, path.find_last_of('.')) + BINARY_EXTENSION;

    if(std::filesystem::exists(binaryFile))
    {
        this->loadModelBinaryFile(binaryFile);
    }
    else
    {
        const aiScene* scene = _assimpImporter.ReadFile(path, aiProcess_JoinIdenticalVertices | aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace);

        if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            std::cout << "ERROR::ASSIMP::" << _assimpImporter.GetErrorString() << '\n';
            return this;
        }

        const std::string shortName = scene->GetShortFilename(path.c_str());
        const std::string folder    = path.substr(0, path.length() - shortName.length());

        this->processNode(scene->mRootNode, scene, folder);
        this->writeBinaryFile(binaryFile);
    }

    for(auto& component : _components)
    {
        this->buildVao(component.get());
    }

    return this;
}

Model3D::Component* DrawMesh::processMesh(const aiMesh* mesh, const aiScene* scene, const std::string& folder) const
{
    std::vector<VAO::Vertex> vertices(mesh->mNumVertices);
    std::vector<GLuint>      indices(mesh->mNumFaces * 4);

    // Vertices
    const int numVertices = static_cast<int>(mesh->mNumVertices);

    for(int i = 0; i < numVertices; i++)
    {
        VAO::Vertex vertex;
        vertex._position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
        vertex._normal   = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
        if(mesh->mTextureCoords[0])
            vertex._textCoord = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);

        vertices[i] = vertex;
    }

    // Indices
    for(unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        const aiFace face = mesh->mFaces[i];
        for(unsigned int j = 0; j < face.mNumIndices; j++)
            indices[i * 4 + j] = face.mIndices[j];

        indices[i * 4 + 3] = RESTART_PRIMITIVE_INDEX;
    }

    Component* component                   = new Component;
    component->_vertices                   = std::move(vertices);
    component->_indices[VAO::IBO_TRIANGLE] = std::move(indices);
    component->completeTopology();

    return component;
}

void DrawMesh::processNode(const aiNode* node, const aiScene* scene, const std::string& folder)
{
    for(unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        _components.push_back(std::unique_ptr<Component>(this->processMesh(mesh, scene, folder)));
    }

    for(unsigned int i = 0; i < node->mNumChildren; i++)
    {
        this->processNode(node->mChildren[i], scene, folder);
    }
}
}    // namespace GDSA::Render
