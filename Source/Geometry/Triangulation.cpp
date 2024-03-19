#include "Triangulation.h"

#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

Triangulation::Triangulation(PointCloud* pointCloud)
{
    typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
    typedef CGAL::Delaunay_triangulation_2<Kernel>              Delaunay;
    typedef CGAL::Segment_2<Kernel>                             Segment;
    typedef CGAL::Ray_2<Kernel>                                 Ray;
    typedef Delaunay::Edge_iterator                             EdgeIterator;

    Delaunay triangulation;
    for(auto& point : pointCloud->getPoints())
    {
        triangulation.insert({point.getX(), point.getY()});
    }

    EdgeIterator edgeIterator = triangulation.edges_begin();
    for(; edgeIterator != triangulation.edges_end(); ++edgeIterator)
    {
        CGAL::Object o = triangulation.dual(edgeIterator);
        if(CGAL::object_cast<Segment>(&o))
        {
            Vect2d orig(CGAL::object_cast<Segment>(&o)->source().x(), CGAL::object_cast<Segment>(&o)->source().y());
            Vect2d dest(CGAL::object_cast<Segment>(&o)->target().x(), CGAL::object_cast<Segment>(&o)->target().y());
            _edges.push_back(std::pair(new SegmentLine(orig, dest), true));
        }
        else if(CGAL::object_cast<Ray>(&o))
        {
            Vect2d orig(CGAL::object_cast<Ray>(&o)->source().x(), CGAL::object_cast<Ray>(&o)->source().y());
            Vect2d dest(CGAL::object_cast<Ray>(&o)->source().x() + CGAL::object_cast<Ray>(&o)->direction().dx() * 1000, CGAL::object_cast<Ray>(&o)->source().y() + CGAL::object_cast<Ray>(&o)->direction().dy() * 1000);
            _edges.push_back(std::pair(new SegmentLine(orig, dest), false));
        }
    }
}
