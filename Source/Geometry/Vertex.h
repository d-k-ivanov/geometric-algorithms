#pragma once

#include "Point.h"
#include "SegmentLine.h"

#include <ostream>

namespace GDSA::Geometry
{
class Polygon;    // Forward declaration due to circular reference betwen both classes

class Vertex : public Point
{
protected:
    static constexpr int INVALID_POSITION = -1;

    Polygon* _polygon;     //!< Polygon where the vertex belongs to
    int      _position;    //!< Position of the vertex in the polygon

public:
    Vertex();
    Vertex(const Point& point);
    Vertex(const Point& point, Polygon* polygon, int pos = -1);

    ~Vertex() override = default;

    Vertex& operator=(const Vertex& vertex);

    /**
     *  @brief Determines if the vertex is in a concave position of the polygon.
     */
    bool concave();

    /**
     *  @brief Determines if the vertex is in a convex position of the polygon.
     */
    bool convex();

    /**
     *  @brief Returns the point value.
     */
    Point getPoint() const { return {_x, _y}; }

    /**
     *  @brief Returns the polygon associated to this vertex.
     */
    Polygon* getPolygon() const { return _polygon; }

    /**
     *  @brief Returns the position of the current vertex in the polygon, if any.
     */
    int getPositionInPolygon() const { return _position; }

    /**
     *  @brief Next vertex in counterclockwise order.
     */
    Vertex next() const;

    /**
     *  @brief Next edge in counterclockwise order.
     */
    SegmentLine nextEdge();

    /**
     *  @brief Overriding cout call.
     */
    friend std::ostream& operator<<(std::ostream& os, const Vertex& vertex);

    /**
     *  @brief Next vertex in clockwise order.
     */
    Vertex previous() const;

    /**
     *  @brief Next edge in clockwise order.
     */
    SegmentLine previousEdge();

    /**
     *  @brief Modifies the coordinates of the vertex.
     */
    void setPoint(Point& point)
    {
        _x = point.getX();
        _y = point.getY();
    }

    /**
     *  @brief Modifies the polygon associated to this vertex.
     */
    void setPolygon(Polygon* polygon) { _polygon = polygon; }

    /**
     *  @brief Modifies the position of the vertex in the polygon, if any.
     */
    void setPosition(int pos) { _position = pos; }
};
}    // namespace GDSA::Geometry
