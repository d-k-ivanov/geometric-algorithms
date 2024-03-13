#include "Vect3d.h"

#include "BasicGeometry.h"
#include "Triangle3d.h"

#include <iostream>
#include <string>
#include <vector>

Vect3d::Vect3d()
{
    this->setVert(FLT_MAX, FLT_MAX, FLT_MAX);
}

Vect3d::Vect3d(const double value)
{
    this->_x = value;
    this->_y = value;
    this->_z = value;
}

Vect3d::Vect3d(const double x, const double y, const double z)
{
    this->_x = x;
    this->_y = y;
    this->_z = z;
}

Vect3d::Vect3d(const Vect3d& vector)
{
    _x = vector._x;
    _y = vector._y;
    _z = vector._z;
}

Vect3d::~Vect3d()
{
}

double Vect3d::getX() const
{
    return _x;
}

double Vect3d::getY() const
{
    return _y;
}

double Vect3d::getZ() const
{
    return _z;
}

std::vector<double> Vect3d::getVert() const
{
    return {getX(), getY(), getZ()};
}

void Vect3d::setX(double x)
{
    this->_x = x;
}

void Vect3d::setY(double y)
{
    this->_y = y;
}

void Vect3d::setZ(double z)
{
    this->_z = z;
}

void Vect3d::setVert(double x, double y, double z)
{
    this->_x = x;
    this->_y = y;
    this->_z = z;
}

Vect3d& Vect3d::operator=(const Vect3d& vector)
{
    this->_x = vector._x;
    this->_y = vector._y;
    this->_z = vector._z;

    return *this;
}

bool Vect3d::operator==(const Vect3d& vector)
{
    return BasicGeometry::equal(getX(), vector._x) && BasicGeometry::equal(getY(), vector._y) && BasicGeometry::equal(getZ(), vector._z);
}

bool Vect3d::operator!=(const Vect3d& vector)
{
    return !(this->operator==(vector));
}

std::ostream& operator<<(std::ostream& os, const Vect3d& vec)
{
    os << "Coordinate X: " << std::to_string(vec._x) << ", coordinate Y: " << std::to_string(vec._y) << ", coordinate Z: " << std::to_string(vec._z);
    return os;
}

Vect3d Vect3d::operator/=(const Vect3d& b)
{
    this->_x /= b._x;
    this->_y /= b._y;
    this->_z /= b._z;

    return *this;
}

Vect3d Vect3d::operator/(const Vect3d& b)
{
    return *this /= b;
}

Vect3d Vect3d::scalarDiv(double value) const
{
    return {getX() / value, getY() / value, getZ() / value};
}

Vect3d Vect3d::operator*=(const Vect3d& b)
{
    this->_x *= b._x;
    this->_y *= b._y;
    this->_z *= b._z;

    return *this;
}

Vect3d Vect3d::operator*(const Vect3d& b)
{
    return *this *= b;
}

Vect3d Vect3d::scalarMul(const double value) const
{
    return {getX() * value, getY() * value, getZ() * value};
}

Vect3d Vect3d::operator+=(const Vect3d& b)
{
    this->_x += b._x;
    this->_y += b._y;
    this->_z += b._z;

    return *this;
}

Vect3d Vect3d::operator+(const Vect3d& b)
{
    return *this += b;
}

Vect3d Vect3d::add(Vect3d& b) const
{
    return {getX() + b.getX(), getY() + b.getY(), getZ() + b.getZ()};
}

Vect3d Vect3d::operator-=(const Vect3d& b)
{
    this->_x -= b._x;
    this->_y -= b._y;
    this->_z -= b._z;

    return *this;
}

Vect3d Vect3d::operator-(const Vect3d& b)
{
    return *this -= b;
}

Vect3d Vect3d::sub(const Vect3d& b) const
{
    return {getX() - b.getX(), getY() - b.getY(), getZ() - b.getZ()};
}

bool Vect3d::collinear(Vect3d& a, Vect3d& b)
{
    Triangle3d tr(a, b, *this);

    return BasicGeometry::equal(tr.area(), BasicGeometry::EPSILON);
}

double Vect3d::distance(const Vect3d& p) const
{
    return std::sqrt(std::pow(this->getX() - p.getX(), 2) + std::pow(this->getY() - p.getY(), 2) + std::pow(this->getZ() - p.getZ(), 2));
}

double Vect3d::module() const
{
    return sqrt(getX() * getX() + getY() * getY() + getZ() * getZ());
}

double Vect3d::dot(const Vect3d& v) const
{
    return (getX() * v.getX() + getY() * v.getY() + getZ() * v.getZ());
}

Vect3d Vect3d::xProduct(const Vect3d& b) const
{
    return {getY() * b.getZ() - getZ() * b.getY(), getZ() * b.getX() - getX() * b.getZ(), getX() * b.getY() - getY() * b.getX()};
}

void Vect3d::getPlane(const Vect3d& v, Vect3d& n, double& d) const
{
    n = v.sub(*this);
    d = (std::pow(v.getX(), 2) - std::pow(this->getX(), 2) + std::pow(v.getY(), 2) - std::pow(this->getY(), 2) + std::pow(v.getZ(), 2) - std::pow(this->getZ(), 2)) / 2.0;
}
