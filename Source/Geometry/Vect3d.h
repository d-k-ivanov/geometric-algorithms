#pragma once

#include <iostream>
#include <vector>

/**
 *  @brief Base class for any structure which needs 3 coordinates (point, vector...).
 */
class Vect3d
{
public:
    union
    {
        double _x, _r;
    };
    union
    {
        double _y, _g;
    };
    union
    {
        double _z, _b;
    };

    /**
     *  @brief Default constructor.
     */
    Vect3d();

    /**
     *  @brief Constructors.
     */
    Vect3d(double value);
    Vect3d(double x, double y, double z);

    /**
     *  @brief Copy constructor.
     */
    Vect3d(const Vect3d& vector);

    /**
     *  @brief Destructor.
     */
    virtual ~Vect3d();

    /**
     *  @brief Returns the X coordinate.
     */
    double getX() const;

    /**
     *  @brief Returns the Y coordinate.
     */
    double getY() const;

    /**
     *  @brief Returns the Z coordinate.
     */
    double getZ() const;

    /**
     *  @brief Returns the coordinates of this vector as an array.
     */
    std::vector<double> getVert() const;

    /**
     *  @brief Modifies the X coordinate.
     */
    void setX(double x);

    /**
     *  @brief Modifies the Y coordinate.
     */
    void setY(double y);

    /**
     *  @brief Modifies the Z coordinate.
     */
    void setZ(double z);

    /**
     *  @brief Modifies all the vector values.
     */
    void setVert(double x, double y, double z);

    /**
     *  @brief Assignment operator.
     */
    Vect3d& operator=(const Vect3d& vector);

    /**
     *  @brief Checks if two vectors are equal.
     */
    virtual bool operator==(const Vect3d& vector);

    /**
     *  @brief Checks if two vectors are distinct.
     */
    virtual bool operator!=(const Vect3d& vector);

    /**
     *  @brief Shows some information of the vector as debugging info.
     */
    friend std::ostream& operator<<(std::ostream& os, const Vect3d& vec);

    /**
     *  @brief Division of this vector by a scalar value.
     */
    Vect3d operator/=(const Vect3d& b);
    Vect3d operator/(const Vect3d& b);
    Vect3d scalarDiv(double value) const;

    /**
     *  @brief Multiplication of this vector by a scalar value.
     */
    Vect3d operator*=(const Vect3d& b);
    Vect3d operator*(const Vect3d& b);
    Vect3d scalarMul(double value) const;

    /**
     *  @brief Vector sum. a + b.
     */
    Vect3d operator+=(const Vect3d& b);
    Vect3d operator+(const Vect3d& b);
    Vect3d add(const Vect3d& b) const;

    Vect3d operator-=(const Vect3d& b);
    Vect3d operator-(const Vect3d& b);
    Vect3d sub(const Vect3d& b) const;

    /**
     *  @brief Vector normalization.
     */
    Vect3d normalize() const;

    /**
     *  @brief Checks if a, b and this vector are on a same line.
     */
    bool collinear(Vect3d& a, Vect3d& b);

    /**
     *  @brief Distance between points.
     */
    double distance(const Vect3d& p) const;

    /**
     *  @brief Returns the module of the vector.
     */
    double module() const;

    /**
     *  @brief Dot product.
     */
    double dot(const Vect3d& v) const;

    /**
     *  @brief Vectorial product.
     */
    Vect3d xProduct(const Vect3d& b) const;

    /**
     *  @brief Return by reference the normal of the equidistant from two points plane and the value of the independent term.
     */
    void getPlane(const Vect3d& v, Vect3d& n, double& d) const;
};
