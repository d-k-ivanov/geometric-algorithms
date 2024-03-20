#include "TriangleModel.h"

#include <GL/glew.h>

#include <filesystem>
#include <fstream>
#include <iostream>

namespace GDSA::Geometry
{
TriangleModel::TriangleModel(const std::string& pathfile)
{
    const std::string binaryFile = pathfile.substr(0, pathfile.find_last_of('.')) + BINARY_EXTENSION;

    if(std::filesystem::exists(binaryFile))
    {
        this->loadModelBinaryFile(binaryFile);
    }
    else
    {
        Assimp::Importer assimpImporter;
        const aiScene*   scene = assimpImporter.ReadFile(pathfile, aiProcess_JoinIdenticalVertices | aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace);

        if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            std::cout << "ERROR::ASSIMP::" << assimpImporter.GetErrorString() << '\n';
            return;
        }

        const std::string shortName = scene->GetShortFilename(pathfile.c_str());
        const std::string folder    = pathfile.substr(0, pathfile.length() - shortName.length());

        this->processNode(scene->mRootNode, scene, folder);
        this->writeBinaryFile(binaryFile);

        assimpImporter.FreeScene();
    }
}

TriangleModel::~TriangleModel()
{
    _vertices.clear();
    _normals.clear();
    _textCoordinates.clear();
}

PointCloud3d TriangleModel::getCloud() const
{
    return PointCloud3d(_vertices);
}

Triangle3d TriangleModel::getFace(const unsigned index) const
{
    if(index > this->numTriangles())
        return {};

    Vect3d a = _vertices[_indices[static_cast<size_t>(index) * 3]];
    Vect3d b = _vertices[_indices[static_cast<size_t>(index) * 3 + 1]];
    Vect3d c = _vertices[_indices[static_cast<size_t>(index) * 3 + 2]];

    return {a, b, c};
}

std::vector<Triangle3d> TriangleModel::getFaces() const
{
    std::vector<Triangle3d> result;

    result.reserve(this->numTriangles());
    for(unsigned index = 0; index < this->numTriangles(); index++)
    {
        result.push_back(getFace(index));
    }

    return result;
}

size_t TriangleModel::numTriangles() const
{
    return _indices.size() / 3;
}

AABB TriangleModel::getAABB()
{
    double minX = DBL_MAX;
    double minY = DBL_MAX;
    double minZ = DBL_MAX;
    double maxX = DBL_MIN;
    double maxY = DBL_MIN;
    double maxZ = DBL_MIN;

    for(const auto& vertice : *this->getVertices())
    {
        // if(vertice.getX() > maxX)
        //     maxX = vertice.getX();
        // if(vertice.getY() > maxY)
        //     maxY = vertice.getY();
        // if(vertice.getZ() > maxZ)
        //     maxZ = vertice.getZ();
        // if(vertice.getX() < minX)
        //     minX = vertice.getX();
        // if(vertice.getY() < minY)
        //     minY = vertice.getY();
        // if(vertice.getZ() < minZ)
        //     minZ = vertice.getZ();

        maxX = BasicGeometry::max2(vertice.getX(), maxX);
        minX = BasicGeometry::min2(vertice.getX(), minX);
        maxY = BasicGeometry::max2(vertice.getY(), maxY);
        minY = BasicGeometry::min2(vertice.getY(), minY);
        maxZ = BasicGeometry::max2(vertice.getZ(), maxZ);
        minZ = BasicGeometry::min2(vertice.getZ(), minZ);
    }

    const Vect3d min(minX, minY, minZ);
    const Vect3d max(maxX, maxY, maxZ);
    return {min, max};
}

void TriangleModel::computeTangents()
{
    Vect3d* tan1 = new Vect3d[_vertices.size()];
    memset(tan1, 0, _vertices.size() * sizeof(Vect3d));
    const int numTriangles = _face.size();

    for(long i = 0; i < numTriangles; i++)
    {
        const long meshIndex = i * 4;
        const long index1 = _face[i]._indices[0], index2 = _face[i]._indices[1], index3 = _face[i]._indices[2];

        const Vect3d &v1 = _vertices[index1], v2 = _vertices[index2], v3 = _vertices[index3];
        const Vect2d &w1 = _textCoordinates[index1], w2 = _textCoordinates[index2], w3 = _textCoordinates[index3];

        const double x1 = v2._x - v1._x, x2 = v3._x - v1._x;
        const double y1 = v2._y - v1._y, y2 = v3._y - v1._y;
        const double z1 = v2._z - v1._z, z2 = v3._z - v1._z;

        const double s1 = w2._x - w1._x, s2 = w3._x - w1._x;
        const double t1 = w2._y - w1._y, t2 = w3._y - w1._y;

        const double r = 1.0f / (s1 * t2 - s2 * t1);
        const Vect3d sdir((t2 * x1 - t1 * x2) * r, (t2 * y1 - t1 * y2) * r, (t2 * z1 - t1 * z2) * r);

        tan1[index1] += sdir;
        tan1[index2] += sdir;
        tan1[index3] += sdir;
    }

    _tangents = std::vector<Vect3d>(_vertices.size());    // Resize

    for(long i = 0; i < _vertices.size(); i++)
    {
        const Vect3d& n = _normals[i];
        const Vect3d& t = tan1[i];

        _tangents[i] = t.sub(n.scalarMul(n.dot(t))).normalize();    // Gram-Schmidt orthogonalize
    }

    delete[] tan1;
}

Render::DrawVoxelization* TriangleModel::voxelize() const
{
    glm::vec3* positions = new glm::vec3[_vertices.size()];
    for(int vertexIdx = 0; vertexIdx < _vertices.size(); ++vertexIdx)
    {
        positions[vertexIdx] = glm::vec3(_vertices[vertexIdx].getX(), _vertices[vertexIdx].getY(), _vertices[vertexIdx].getZ());
    }

    Render::DrawVoxelization* voxelization = new Render::DrawVoxelization(positions, static_cast<int>(_vertices.size()), glm::vec3(.01f));
    delete[] positions;

    return voxelization;
}

void TriangleModel::loadModelBinaryFile(const std::string& path)
{
    std::ifstream fin(path, std::ios::in | std::ios::binary);
    if(!fin.is_open())
    {
        std::cout << "Failed to open the binary file " << path << "!\n";
        return;
    }

    size_t numVertices, numTextCoords, numIndices;

    fin.read(reinterpret_cast<char*>(&numVertices), sizeof(size_t));
    fin.read(reinterpret_cast<char*>(&numTextCoords), sizeof(size_t));
    this->_vertices.resize(numVertices);
    this->_normals.resize(numVertices);
    this->_textCoordinates.resize(numTextCoords);

    fin.read(reinterpret_cast<char*>(this->_vertices.data()), sizeof(Vect3d) * numVertices);
    fin.read(reinterpret_cast<char*>(this->_normals.data()), sizeof(Vect3d) * numVertices);
    if(numTextCoords)
        fin.read(reinterpret_cast<char*>(this->_textCoordinates.data()), sizeof(Vect2d) * numVertices);

    fin.read(reinterpret_cast<char*>(&numIndices), sizeof(size_t));
    if(numIndices)
    {
        this->_indices.resize(numIndices);
        fin.read(reinterpret_cast<char*>(this->_indices.data()), sizeof(GLuint) * numIndices);
    }

    fin.close();
}

void TriangleModel::processMesh(const aiMesh* mesh, const aiScene* scene, const std::string& folder)
{
    // Vertices
    const unsigned baseIndex   = this->_vertices.size();
    const int      numVertices = static_cast<int>(mesh->mNumVertices);

    for(int i = 0; i < numVertices; i++)
    {
        this->_vertices.emplace_back(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
        this->_normals.emplace_back(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
        if(mesh->mTextureCoords[0])
            this->_textCoordinates.emplace_back(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
    }

    // Indices
    for(unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        const aiFace face = mesh->mFaces[i];
        for(unsigned int j = 0; j < face.mNumIndices; j++)
            this->_indices.push_back(face.mIndices[j] + baseIndex);
    }
}

void TriangleModel::processNode(const aiNode* node, const aiScene* scene, const std::string& folder)
{
    for(unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        const aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        this->processMesh(mesh, scene, folder);
    }

    for(unsigned int i = 0; i < node->mNumChildren; i++)
    {
        this->processNode(node->mChildren[i], scene, folder);
    }
}

void TriangleModel::writeBinaryFile(const std::string& path)
{
    std::ofstream fout(path, std::ios::out | std::ios::binary);
    if(!fout.is_open())
    {
        std::cout << "Failed to write the binary file!" << std::endl;
    }

    size_t numVertices = this->_vertices.size(), numTextCoordinates = this->_textCoordinates.size();

    fout.write(reinterpret_cast<char*>(&numVertices), sizeof(size_t));
    fout.write(reinterpret_cast<char*>(&numTextCoordinates), sizeof(size_t));
    fout.write(reinterpret_cast<char*>(this->_vertices.data()), numVertices * sizeof(Vect3d));
    fout.write(reinterpret_cast<char*>(this->_normals.data()), numVertices * sizeof(Vect3d));
    if(numTextCoordinates)
        fout.write(reinterpret_cast<char*>(this->_textCoordinates.data()), numTextCoordinates * sizeof(Vect2d));

    size_t numIndices = this->_indices.size();
    fout.write(reinterpret_cast<char*>(&numIndices), sizeof(size_t));
    if(numIndices)
        fout.write(reinterpret_cast<char*>(this->_indices.data()), numIndices * sizeof(GLuint));

    fout.close();
}

/// [Face]
TriangleModel::Face::Face(TriangleModel* mesh)
    : _triangleModel(mesh)
{
    _indices[0] = _indices[1] = _indices[2] = 0;
}

TriangleModel::Face::Face(const unsigned i1, const unsigned i2, const unsigned i3, TriangleModel* mesh)
    : _triangleModel(mesh)
{
    this->setIndexes(i1, i2, i3);
}

TriangleModel::Face::Face(const Face& face)
    : _triangleModel(face._triangleModel)
{
    this->setIndexes(face._indices[0], face._indices[1], face._indices[2]);
}

TriangleModel::Face& TriangleModel::Face::operator=(const Face& face)
{
    this->setIndexes(face._indices[0], face._indices[1], face._indices[2]);
    this->_triangleModel = face._triangleModel;

    return *this;
}

Vect2d TriangleModel::Face::getTextCoord(const Vect3d& minPoint)
{
    // Christer Ericson's Real-Time Collision Detection:  Cramer's rule

    double u, v, w;

    const Vect3d v0 = _triangle.getB().sub(_triangle.getA()), v1 = _triangle.getC().sub(_triangle.getA()), v2 = minPoint.sub(_triangle.getA());
    const double d00   = v0.dot(v0);
    const double d01   = v0.dot(v1);
    const double d11   = v1.dot(v1);
    const double d20   = v2.dot(v0);
    const double d21   = v2.dot(v1);
    const double denom = d00 * d11 - d01 * d01;
    v                  = (d11 * d20 - d01 * d21) / denom;
    w                  = (d00 * d21 - d01 * d20) / denom;
    u                  = 1.0f - v - w;

    return _triangleModel->_textCoordinates[_indices[0]].ScalarMult(u) + _triangleModel->_textCoordinates[_indices[1]].ScalarMult(v) + _triangleModel->_textCoordinates[_indices[2]].ScalarMult(w);
}

void TriangleModel::Face::setIndexes(unsigned i1, unsigned i2, unsigned i3)
{
    _indices[0] = i1;
    _indices[1] = i2;
    _indices[2] = i3;

    this->updateTriangle();
}

Triangle3d* TriangleModel::Face::updateTriangle()
{
    _triangle = Triangle3d(_triangleModel->_vertices[_indices[0]], _triangleModel->_vertices[_indices[1]], _triangleModel->_vertices[_indices[2]]);

    return &_triangle;
}
}    // namespace GDSA::Geometry