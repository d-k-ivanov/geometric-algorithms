#pragma once

#include "PointCloud3d.h"
#include "Triangle3d.h"
#include "Vect2d.h"
#include "Vect3d.h"

#include "Rendering/DrawVoxelization.h"

#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>

#define BINARY_EXTENSION ".bin"

namespace GDSA::Geometry
{
class TriangleModel
{
public:
    class Face;

private:
    std::vector<Vect3d>   _vertices;
    std::vector<Vect3d>   _normals;
    std::vector<Vect3d>   _tangents;
    std::vector<Vect2d>   _textCoordinates;
    std::vector<unsigned> _indices;

    // [Topology]
    std::vector<Face> _face;    //!< Mesh faces

protected:
    void loadModelBinaryFile(const std::string& path);
    void processMesh(const aiMesh* mesh, const aiScene* scene, const std::string& folder);
    void processNode(const aiNode* node, const aiScene* scene, const std::string& folder);
    void writeBinaryFile(const std::string& path);

public:
    TriangleModel(const std::string& pathfile);
    TriangleModel(const TriangleModel& orig) = delete;
    virtual ~TriangleModel();

    PointCloud3d            getCloud() const;
    Triangle3d              getFace(unsigned index) const;
    std::vector<Triangle3d> getFaces() const;
    std::vector<Vect3d>*    getVertices() { return &_vertices; }
    std::vector<Vect3d>*    getNormals() { return &_normals; }
    std::vector<Vect2d>*    getTextureCoordinates() { return &_textCoordinates; }
    std::vector<unsigned>*  getIndices() { return &_indices; }
    size_t                  numTriangles() const;
    AABB                    getAABB();

    /**
     *	@brief Computes tangents for every point of the triangle mesh, since it's never included by default.
     */
    void computeTangents();

    Render::DrawVoxelization* voxelize() const;
};

class TriangleModel::Face
{
public:
    enum FacePlaneRelation
    {
        NO_RELATION,
        POSITIVE,
        NEGATIVE,
        POINT_INT,
        SEGMENT_INT,
        COPLANAR
    };

public:
    unsigned       _indices[3];       //!< Indexes of vertices
    Triangle3d     _triangle;         //!< Core
    TriangleModel* _triangleModel;    //!< Mesh where those vertices belong to

public:
    Face(TriangleModel* mesh);
    Face(const unsigned i1, const unsigned i2, const unsigned i3, TriangleModel* mesh);
    Face(const Face& face);
    virtual ~Face() = default;

    Face& operator=(const Face& face);

    /**
     *	@return Triangle normal.
     */
    Vect3d getNormal() const { return _triangle.normal(); }

    /**
     *	@return Triangle tangent, even though we return the tangent of any vertex from the triangle.
     */
    Vect3d getTangent() const { return _triangleModel->_tangents[_indices[0]]; }

    /**
     *	@return Texture coordinate for two parametric values which tells us the position inside the triangle.
     */
    Vect2d getTextCoord(const Vect3d& minPoint);

    /**
     *	@brief Returns a vertex index.
     */
    unsigned getVertexIndex(const int i) const { return _indices[i]; }

    /**
     *	@brief Modifies the indexes of vertices.
     *	@param i1, i2, i3 Indexes of vertices.
     */
    void setIndexes(unsigned i1, unsigned i2, unsigned i3);

    /**
     *	@brief Updates the triangle data as the vertices could have changed.
     *	@return Updated triangle.
     */
    Triangle3d* updateTriangle();
};
}    // namespace GDSA::Geometry
