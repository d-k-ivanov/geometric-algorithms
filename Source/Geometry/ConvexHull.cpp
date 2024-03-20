#include "ConvexHull.h"

#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Delaunay_triangulation_3.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Surface_mesh.h>
#include <CGAL/convex_hull_2.h>
#include <CGAL/convex_hull_3.h>
#include <CGAL/convex_hull_3_to_face_graph.h>

namespace GDSA::Geometry
{
ConvexHull::ConvexHull(PointCloud* pointCloud)
{
    typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;

    typedef Kernel::Point_2                        Point_2;
    typedef CGAL::Delaunay_triangulation_2<Kernel> Delaunay;
    typedef std::vector<Point_2>                   Points;

    Points points, result;
    for(auto& point : pointCloud->getPoints())
    {
        points.emplace_back(point.getX(), point.getY());
    }

    Delaunay triangulation;
    triangulation.insert(points.begin(), points.end());

    // Points on the convex hull:
    CGAL::convex_hull_2(points.begin(), points.end(), std::back_inserter(result));

    for(const auto& pointVector : result)
    {
        _pointSequence2D.emplace_back(pointVector.x(), pointVector.y());
    }

    for(Delaunay::Finite_faces_iterator it = triangulation.finite_faces_begin(); it != triangulation.finite_faces_end(); ++it)
    {
        Vect2d p1(it->vertex(0)->point().x(), it->vertex(0)->point().y());
        Vect2d p2(it->vertex(1)->point().x(), it->vertex(1)->point().y());
        Vect2d p3(it->vertex(2)->point().x(), it->vertex(2)->point().y());
        _triangles2D.emplace_back(p1, p2, p3);
    }
}

ConvexHull::ConvexHull(PointCloud3d* pointCloud)
{
    typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;

    typedef Kernel::Point_3                        Point_3;
    typedef CGAL::Delaunay_triangulation_3<Kernel> Delaunay;
    typedef Delaunay::Vertex_handle                Vertex_handle;
    typedef CGAL::Surface_mesh<Point_3>            Surface_mesh;
    typedef std::vector<Point_3>                   Points;

    Points points;
    for(auto& point : pointCloud->getPoints())
    {
        points.emplace_back(point.getX(), point.getY(), point.getZ());
    }

    std::vector<std::size_t> indices(points.size());
    for(std::size_t i = 0; i < points.size(); ++i)
    {
        indices[i] = i;
    }

    Delaunay triangulation;
    triangulation.insert(points.begin(), points.end());

    std::vector<Vertex_handle> vertices;
    triangulation.incident_vertices(triangulation.infinite_vertex(), std::back_inserter(vertices));

    Surface_mesh result;
    CGAL::convex_hull_3_to_face_graph(triangulation, result);

    for(const auto edge : edges(result))
    {
        Point_3 p1 = result.point(source(edge, result));
        Point_3 p2 = result.point(target(edge, result));
        Vect3d  v1(p1.x(), p1.y(), p1.z());
        Vect3d  v2(p2.x(), p2.y(), p2.z());
        _segments3D.emplace_back(v1, v2);
    }

    for(const auto face : result.faces())
    {
        for(auto vertex : result.vertices_around_face(result.halfedge(face)))
        {
            Point_3 p = result.point(vertex);
            _pointSequence3D.emplace_back(p.x(), p.y(), p.z());
        }
    }

    for(size_t i = 0; i < _pointSequence3D.size(); i++)
    {
        Vect3d p1 = _pointSequence3D[i];
        Vect3d p2 = _pointSequence3D[(i + 1) % _pointSequence3D.size()];
        Vect3d p3 = _pointSequence3D[(i + 2) % _pointSequence3D.size()];
        _triangles3D.emplace_back(p1, p2, p3);
    }
}

ConvexHull::ConvexHull(TriangleModel* model)
{
    typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;

    typedef Kernel::Point_3                        Point_3;
    typedef CGAL::Delaunay_triangulation_3<Kernel> Delaunay;
    typedef Delaunay::Vertex_handle                Vertex_handle;
    typedef CGAL::Surface_mesh<Point_3>            Surface_mesh;
    typedef std::vector<Point_3>                   Points;

    Points points;

    for(auto& triangle : model->getFaces())
    {
        points.emplace_back(triangle.getA().getX(), triangle.getA().getY(), triangle.getA().getZ());
        points.emplace_back(triangle.getB().getX(), triangle.getB().getY(), triangle.getB().getZ());
        points.emplace_back(triangle.getC().getX(), triangle.getC().getY(), triangle.getC().getZ());
    }

    std::vector<std::size_t> indices(points.size());
    for(std::size_t i = 0; i < points.size(); ++i)
    {
        indices[i] = i;
    }

    Delaunay triangulation;
    triangulation.insert(points.begin(), points.end());

    std::vector<Vertex_handle> vertices;
    triangulation.incident_vertices(triangulation.infinite_vertex(), std::back_inserter(vertices));

    Surface_mesh result;
    CGAL::convex_hull_3_to_face_graph(triangulation, result);

    for(const auto edge : edges(result))
    {
        Point_3 p1 = result.point(source(edge, result));
        Point_3 p2 = result.point(target(edge, result));
        Vect3d  v1(p1.x(), p1.y(), p1.z());
        Vect3d  v2(p2.x(), p2.y(), p2.z());
        _segments3D.emplace_back(v1, v2);
    }

    for(const auto face : result.faces())
    {
        for(auto vertex : result.vertices_around_face(result.halfedge(face)))
        {
            Point_3 p = result.point(vertex);
            _pointSequence3D.emplace_back(p.x(), p.y(), p.z());
        }
    }

    for(size_t i = 0; i < _pointSequence3D.size(); i++)
    {
        Vect3d p1 = _pointSequence3D[i];
        Vect3d p2 = _pointSequence3D[(i + 1) % _pointSequence3D.size()];
        Vect3d p3 = _pointSequence3D[(i + 2) % _pointSequence3D.size()];
        _triangles3D.emplace_back(p1, p2, p3);
    }
}
}   // namespace GDSA::Geometry
