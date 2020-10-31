#pragma once

#include <iostream>
#include <vector>
#include <cmath>


namespace Geom {

    static float PRECISION = 1e-4;

    struct Solution;
    struct Point;
    struct Vector;
    struct Interval;
    struct Line;
    struct Triangle;
    struct Plane;

    struct Solution {
        float x1 = NAN, x2 = NAN;
        bool isValid() const { return (!isnanf(x1) && !isnanf(x2)); }
    };

    struct Point {
        float x = NAN, y = NAN, z = NAN;

        Point() = default;

        explicit Point(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}


        bool isValid() const { return !(isnanf(x) || isnanf(y) || isnanf(z)); }
        float Abs() const { return std::sqrt(x * x + y * y + z * z); }
        Point operator + (Point& p) const;
        Point operator - (Point& p) const;
        Point operator / (float a) const;
        Point operator * (float a) const;
    };

    struct Vector {
        Point V;

        Vector() = default;

        explicit Vector(Point begin, Point end) {
            V.x = end.x - begin.x;
            V.y = end.y - begin.y;
            V.z = end.z - begin.z;
        }

        explicit Vector(float x, float y, float z) : V(x, y, z) {}

        bool IsPerpendicularToOther (Vector other) const;
        bool IsCollinearToOther(Vector other) const;
        bool IsZero() const;
        bool isValid() const { return V.isValid(); }
        Vector VectorMult(Vector second_vec) const;
        float ScalarMult (Vector second_vec) const;
        float Abs() const { return V.Abs(); }
        Vector Normalize() const;
    };

    struct Interval {
        Point C1, C2;
        Interval() = default;
        explicit Interval (Point C1_, Point C2_) { C1 = C1_; C2 = C2_; }
        bool isValid() const { return (C1.isValid() && C2.isValid()); }
        bool IsPointBelongsToInterval (Point M) const;
        bool IsIntersectWithOther (Interval other) const;
        bool IsOverlapWithOther (Interval other) const;
        float Length() const { Vector v{C1, C2}; return v.Abs(); }
    };

    struct Line {
        Point M;
        Vector vec;

        Line() : M(), vec() {}

        explicit Line(Point A1, Point A2) { M = A1; vec = Vector(A1, A2); }
        explicit Line(Point M_, Vector vec_) { M = M_; vec = vec_; }
        bool isValid() const { return (M.isValid() && vec.isValid()); }
        Point IntersectionWithOtherLine (Line other, bool *isCoincident) const;
    };

    struct Triangle {
        unsigned number;
        Point A1, A2, A3;

        Triangle() : number(0), A1(), A2(), A3() {}

        explicit Triangle(unsigned num, Point A1_, Point A2_, Point A3_) : number(num), A1(A1_), A2(A2_), A3(A3_) {}

        explicit Triangle(unsigned num,
                          float x1, float y1, float z1,
                          float x2, float y2, float z2,
                          float x3, float y3, float z3) : Triangle(num,
                                                                   Point {x1, y1, z1},
                                                                   Point {x2, y2, z2},
                                                                   Point {x3, y3, z3}) {}

        bool isValid() const;
        bool IsIntersectWithOther(Triangle other) const;
        bool IsIntersectThePlane (Plane plane) const;
        Interval IntersectWithLine (Line line) const;
        void Dump (std::ostream& os) const;
    };


    struct Plane {
        float A = NAN, B = NAN, C = NAN, D = NAN;

        Plane(Point A1, Point A2, Point A3);
        explicit Plane(Triangle tr): Plane(tr.A1, tr.A2, tr.A3) {}
        bool isValid() const { return !(isnanf(A) || isnanf(B) || isnanf(C) || isnanf(D)); }
        Line IntersectionWithOtherPlane(Plane other) const;
        Point IntersectionWithLine (Line line) const;
        bool IsEqualToOtherPlane (Plane other) const;
        void Dump (std::ostream &os) const;
    };

    float Determinant(float a11, float a12,
                      float a21, float a22);

    Solution SolveTwoEquations (float a11, float a12, float b1,
                             float a21, float a22, float b2);

}
std::ostream& operator << (std::ostream &os, Geom::Triangle& tr);
std::ostream& operator << (std::ostream &os, Geom::Plane& plane);