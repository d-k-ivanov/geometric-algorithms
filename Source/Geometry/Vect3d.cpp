#include "Vect3d.h"

#include "BasicGeometry.h"
#include "Triangle3d.h"

#include <algorithm>
#include <cfloat>
#include <iostream>
#include <string>
#include <vector>

namespace GDSA::Geometry
{
Vect3d::Vect3d()
{
    this->setVert(DBL_MAX, DBL_MAX, DBL_MAX);
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

double Vect3d::getAt(const int index) const
{
    switch(index)
    {
        case 0:
            return getX();
        case 1:
            return getY();
        case 2:
            return getZ();
        default:
            return 0.0;
    }
}

std::vector<double> Vect3d::getVert() const
{
    return {getX(), getY(), getZ()};
}

void Vect3d::setX(const double x)
{
    this->_x = x;
}

void Vect3d::setY(const double y)
{
    this->_y = y;
}

void Vect3d::setZ(const double z)
{
    this->_z = z;
}

void Vect3d::setAt(const int index, const double value)
{
    switch(index)
    {
        case 0:
            setX(value);
            break;
        case 1:
            setY(value);
            break;
        case 2:
            setZ(value);
            break;
        default:
            break;
    }
}

glm::vec3 Vect3d::toGlmVec3() const
{
    return {getX(), getY(), getZ()};
}

void Vect3d::setVert(const double x, const double y, const double z)
{
    this->_x = x;
    this->_y = y;
    this->_z = z;
}

Vect3d& Vect3d::operator=(const Vect3d& vector)
{
    this->_x = vector.getX();
    this->_y = vector.getY();
    this->_z = vector.getZ();

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
    this->_x /= b.getX();
    this->_y /= b.getY();
    this->_z /= b.getZ();

    return {*this};
}

Vect3d Vect3d::operator/(const Vect3d& b)
{
    return Vect3d(*this) /= b;
}

Vect3d Vect3d::div(const Vect3d& b) const
{
    return {getX() / b.getX(), getY() / b.getY(), getZ() / b.getZ()};
}

Vect3d Vect3d::operator/=(const double value)
{
    this->_x /= value;
    this->_y /= value;
    this->_z /= value;

    return {*this};
}

Vect3d Vect3d::operator/(const double value)
{
    return Vect3d(*this) /= value;
}

Vect3d Vect3d::div(const double value) const
{
    return {getX() / value, getY() / value, getZ() / value};
}

Vect3d Vect3d::scalarDiv(const double value) const
{
    return {getX() / value, getY() / value, getZ() / value};
}

Vect3d Vect3d::operator*=(const Vect3d& b)
{
    this->_x *= b.getX();
    this->_y *= b.getY();
    this->_z *= b.getZ();

    return {*this};
}

Vect3d Vect3d::operator*(const Vect3d& b)
{
    return Vect3d(*this) *= b;
}

Vect3d Vect3d::mul(const Vect3d& b) const
{
    return {getX() * b.getX(), getY() * b.getY(), getZ() * b.getZ()};
}

Vect3d Vect3d::operator*=(const double value)
{
    this->_x *= value;
    this->_y *= value;
    this->_z *= value;

    return {*this};
}

Vect3d Vect3d::operator*(const double value)
{
    return Vect3d(*this) *= value;
}

Vect3d Vect3d::mul(const double value) const
{
    return {getX() * value, getY() * value, getZ() * value};
}

Vect3d Vect3d::scalarMul(const double value) const
{
    return {getX() * value, getY() * value, getZ() * value};
}

Vect3d Vect3d::operator+=(const Vect3d& b)
{
    this->_x += b.getX();
    this->_y += b.getY();
    this->_z += b.getZ();

    return {*this};
}

Vect3d Vect3d::operator+(const Vect3d& b)
{
    return Vect3d(*this) += b;
}

Vect3d Vect3d::add(const Vect3d& b) const
{
    return {getX() + b.getX(), getY() + b.getY(), getZ() + b.getZ()};
}

Vect3d Vect3d::operator-=(const Vect3d& b)
{
    this->_x -= b.getX();
    this->_y -= b.getY();
    this->_z -= b.getZ();

    return {*this};
}

Vect3d Vect3d::operator-(const Vect3d& b)
{
    return Vect3d(*this) -= b;
}

Vect3d Vect3d::sub(const Vect3d& b) const
{
    return {getX() - b.getX(), getY() - b.getY(), getZ() - b.getZ()};
}

Vect3d& Vect3d::ceil(const Vect3d& v)
{
    return *new Vect3d(std::ceil(v.getX()), std::ceil(v.getY()), std::ceil(v.getZ()));
}

Vect3d Vect3d::clamp(const Vect3d& v, const Vect3d& min, const Vect3d& max)
{
    return {std::clamp(v.getX(), min.getX(), max.getX()), std::clamp(v.getY(), min.getY(), max.getY()), std::clamp(v.getZ(), min.getZ(), max.getZ())};
}

Vect3d Vect3d::normalize() const
{
    return this->scalarDiv(module());
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
}    // namespace GDSA::Geometry
