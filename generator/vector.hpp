#pragma once
#include <utility>
#include <array>
#include <iostream>
#include <cmath>
#include "metaprogramming_stuff.hpp"
#include "fundamental_types.hpp"



template<std::size_t Dimensions,
         typename Precision,
         typename = EnableIf<(Dimensions > 0)>,
         typename = EnableIf<IsArithmetic<Precision>>>
class Vector {
public:
    static constexpr auto DimensionsNum = Dimensions;
    using PrecisionType = Precision;

public:
    Vector() = default;
    Vector(const Vector& v): parameters {v.parameters} {}
    template<size_t Dim = Dimensions, typename = EnableIf<(Dim == 1)>>
    Vector(const Precision& x): parameters ({x}) {}

    template<size_t Dim = Dimensions, typename = EnableIf<(Dim == 2)>>
    Vector(const Precision& x, const Precision& y):
            parameters ({x, y}) {}

    template<size_t Dim = Dimensions, typename = EnableIf<(Dim == 3)>>
    Vector(const Precision& x, const Precision& y, const Precision& z):
            parameters ({x, y, z}) {}

    template<size_t Dim = Dimensions, typename = EnableIf<(Dim == 4)>>
    Vector(const Precision& x, const Precision& y,
           const Precision& z, const Precision& w):
            parameters ({x, y, z, w}) {}

    Vector(const std::initializer_list<Precision>& l) {
        std::copy(l.begin(), l.end(), parameters.begin());
    }

    void set(const Precision& x, const Precision& y) {
        parameters[0] = x;
        parameters[1] = y;
    }

    Vector& normalize() {
        *this = normalized();
        return *this;
    }

    Vector normalized() const {
        Vector v {*this};
        auto length = getLength();
        if(length == 0) {
            for(auto& coord: v.parameters) {
                coord = 0;
            }
        } else {
            for (auto &coord: v.parameters) {
                coord /= length;
            }
        }
        return v;
    }

    template<size_t Dims = Dimensions, size_t DimsRhs>
    EnableIf<Dims == 3 and DimsRhs == 3, Vector>
    cross(const Vector<DimsRhs, Precision>& v) const {
        auto x = this->y() * v.z() - this->z() * v.y();
        auto y = this->z() * v.x() - this->x() * v.z();
        auto z = this->x() * v.y() - this->y() * v.x();

        return {x, y, z};
    }

    template<size_t Dims = Dimensions>
    EnableIf<Dims == 3, Vector&>
    rotate(f32 angleRad, const Vector<3, Precision>& axe) {
        f32 sinHalfAngle = sinf(angleRad/2);
        f32 cosHalfAngle = cosf(angleRad/2);

        f32 rx = axe.x() * sinHalfAngle;
        f32 ry = axe.y() * sinHalfAngle;
        f32 rz = axe.z() * sinHalfAngle;
        f32 rw = cosHalfAngle;
        Vector<4, f32> rotation {rx, ry, rz, rw};

        Vector<4, f32> conjugate = {-rotation.x(), -rotation.y(),
                                    -rotation.z(), +rotation.w()};
        // ??? ConjugateQ.Normalize();
        Vector<4, f32> w = rotation * (*this) * conjugate;

        parameters[0] = w.x();
        parameters[1] = w.y();
        parameters[2] = w.z();
        return *this;
    }

    Vector operator-() const {
        Vector result = *this;
        for(auto& parameter: result.parameters) {
            parameter = -parameter;
        }
        return result;
    }

    Vector operator*(Precision multiplier) const {
        Vector res = *this;
        res *= multiplier;
        return res;
    }


    Vector& operator*=(Precision multiplier) {
        for(auto& param: parameters) {
            param *= multiplier;
        }
        return *this;
    }



    template<size_t Dims = Dimensions, typename VPrecision>
    Precision operator*(const Vector<Dims, VPrecision>& v) const {
        Precision sum {0};
        for (size_t i = 0; i < Dimensions; i++) {
            sum += parameters.at(i) * v.parameters.at(i);
        }
        return sum;
    }



    template<size_t Dims = Dimensions, size_t DimsRhs>
    EnableIf<Dims == 4 and DimsRhs == Dims,Vector>
    cross(const Vector<DimsRhs, Precision>& v) const {
        auto rw = w() * v.w() - x() * v.x() - y() * v.y() - z() * v.z();
        auto rx = x() * v.w() + w() * v.x() + y() * v.z() - z() * v.y();
        auto ry = y() * v.w() + w() * v.y() + z() * v.x() - x() * v.z();
        auto rz = z() * v.w() + w() * v.z() + x() * v.y() - y() * v.x();
        return {rx, ry, rz, rw};
    }

    template<size_t Dims = Dimensions, size_t DimsRhs>
    EnableIf<Dims == 4 and DimsRhs == 3, Vector>
    cross(const Vector<DimsRhs, Precision>& v) const {
        auto rw = - x() * v.x() - y() * v.y() - z() * v.z();
        auto rx =   w() * v.x() + y() * v.z() - z() * v.y();
        auto ry =   w() * v.y() + z() * v.x() - x() * v.z();
        auto rz =   w() * v.z() + x() * v.y() - y() * v.x();
        return {rx, ry, rz, rw};
    }

    Precision getLength() const {
        Precision length {};
        for(auto& coord: parameters) {
            length += coord*coord;
        }
        return sqrt(length);
    }

    const Precision& x() const {
        return parameters[0];
    }

    template<size_t Dim = Dimensions>
    EnableIf<(Dim > 1), const Precision&> y() const {
        return parameters[1];
    }

    template<size_t Dim = Dimensions>
    EnableIf<(Dim > 2), const Precision&> z() const {
        return parameters[2];
    }

    template<size_t Dim = Dimensions>
    EnableIf<(Dim > 3), const Precision&> w() const {
        return parameters[3];
    }

    void setX(Precision x) {
        parameters[0] = x;
    }

    template<size_t Dim = Dimensions>
    EnableIf<(Dim > 1), void> setY(Precision y) {
        parameters[1] = y;
    }

    template<size_t Dim = Dimensions>
    EnableIf<(Dim > 2), void> setZ(Precision z) {
        parameters[2] = z;
    }

    template<size_t Dim = Dimensions>
    EnableIf<(Dim > 3), void> setW(Precision w) {
        parameters[3] = w;
    }

    const Precision& getParameter(size_t index) const {
        if (index >= parameters.size()) {
            throw std::out_of_range {"Index "+std::to_string(index)+" is out of vector's dimension's num"};
        }
        return parameters[index];
    }

    Vector operator-(const Vector& rhs) const {
        Vector result {*this};
        for(size_t i = 0; i < rhs.parameters.size(); i++) {
            result.parameters[i] -= rhs.parameters[i];
        }
    }

    Vector& operator-=(const Vector& rhs) {
        *this = *this - rhs;
        return *this;
    }

    Vector operator+(const Vector& rhs) const {
        Vector result {*this};
        for(size_t i = 0; i < rhs.parameters.size(); i++) {
            result.parameters[i] += rhs.parameters[i];
        }
        return result;
    }

    Vector& operator+=(const Vector& rhs) {
        *this = *this + rhs;
        return *this;
    }

private:
    std::array<Precision, Dimensions> parameters {};

};


#include <ostream>
template<std::size_t Dimensions,
         typename Precision,
         typename = EnableIf<(Dimensions > 0)>,
         typename = EnableIf<IsArithmetic<Precision>>>
std::ostream& operator <<(std::ostream& out, const Vector<Dimensions, Precision>& v) {
    out << "{";
    for(size_t i = 0; i < Dimensions-1; i++) {
        out << v.getParameter(i) << ", ";
    }
    out << v.getParameter(Dimensions-1) << "}";
    return out;
}

#define VECTOR_SHORTCUT(dimension, precision) using Vector##dimension##precision = Vector<dimension, precision>;

template<typename Precision,
         typename = EnableIf<IsArithmetic<Precision>>>
using Vector1 = Vector<1, Precision>;

VECTOR_SHORTCUT(1, i32)
VECTOR_SHORTCUT(1, u32)
VECTOR_SHORTCUT(1, i16)
VECTOR_SHORTCUT(1, u16)
VECTOR_SHORTCUT(1, i64)
VECTOR_SHORTCUT(1, u64)
VECTOR_SHORTCUT(1, f32)
VECTOR_SHORTCUT(1, f64)


template<typename Precision,
         typename = EnableIf<IsArithmetic<Precision>>>
using Vector2 = Vector<2, Precision>;

VECTOR_SHORTCUT(2, i32)
VECTOR_SHORTCUT(2, u32)
VECTOR_SHORTCUT(2, i16)
VECTOR_SHORTCUT(2, u16)
VECTOR_SHORTCUT(2, i64)
VECTOR_SHORTCUT(2, u64)
VECTOR_SHORTCUT(2, f32)
VECTOR_SHORTCUT(2, f64)

template<typename Precision,
         typename = EnableIf<IsArithmetic<Precision>>>
using Vector3 = Vector<3, Precision>;

VECTOR_SHORTCUT(3, i32)
VECTOR_SHORTCUT(3, u32)
VECTOR_SHORTCUT(3, i16)
VECTOR_SHORTCUT(3, u16)
VECTOR_SHORTCUT(3, i64)
VECTOR_SHORTCUT(3, u64)
VECTOR_SHORTCUT(3, f32)
VECTOR_SHORTCUT(3, f64)

template<typename Precision,
         typename = EnableIf<IsArithmetic<Precision>>>
using Vector4 = Vector<4, Precision>;

VECTOR_SHORTCUT(4, i32)
VECTOR_SHORTCUT(4, u32)
VECTOR_SHORTCUT(4, i16)
VECTOR_SHORTCUT(4, u16)
VECTOR_SHORTCUT(4, i64)
VECTOR_SHORTCUT(4, u64)
VECTOR_SHORTCUT(4, f32)
VECTOR_SHORTCUT(4, f64)
