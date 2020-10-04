#pragma once

#include <iostream>
#include <cmath>


namespace Geom {

    static float PRECISION = 1e-5;

    struct Solution;
    struct Point;
    struct Vector;
    struct Interval;
    struct Line;
    struct Triangle;
    struct Plane;

    struct Solution {
        float x1 = NAN, x2 = NAN;
        bool isValid() const { return (x1 != NAN && x2 != NAN); }
    };

    struct Point {
        float x = NAN, y = NAN, z = NAN;

        Point() = default;

        explicit Point(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}

        bool isValid() const { return (x != NAN && y != NAN && z != NAN); }
        float Abs() const { return std::sqrt(x * x + y * y + z * z); }
        Point operator + (Point& p) const;
        Point operator - (Point& p) const;
        Point operator / (float a) const;
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

        bool IsCollinearToOther(Vector other) const;
        bool IsZero() const;
        bool isValid() const { return V.isValid(); }
        Vector VectorMult(Vector second_vec) const;
        float ScalarMult (Vector second_vec) const;
        float Abs() const { return V.Abs(); }
    };

    struct Interval {
        Point C1, C2;
        Interval() = default;
        explicit Interval (Point C1_, Point C2_) { C1 = C1_; C2 = C2_; }
        bool IsPointBelongsToInterval (Point M) const;
        bool IsIntersectWithOther (Interval other) const;
        bool IsOverlapWithOther (Interval other) const;
    };

    struct Line {
        Point M;
        Vector vec;

        Line() : M(), vec() {}

        explicit Line(Point A1, Point A2) { M = A1; vec = Vector(A1, A2); }
        explicit Line(Point M_, Vector vec_) { M = M_; vec = vec_; }
        bool isValid() const { return (M.isValid() && vec.isValid()); }
        Point IntersectionWithOtherLine (Line other, bool *isCoincedent) const; // надо учитывать, что совпадение прямых - пересечение!
    };

    struct Triangle {
        Point A1, A2, A3;

        Triangle() : A1(), A2(), A3() {}

        explicit Triangle(Point A1_, Point A2_, Point A3_) { A1 = A1_;
                                                             A2 = A2_;
                                                             A3 = A3_; }
        bool isValid() const;
        bool IsIntersectWithOther(Triangle other) const;
        bool IsIntersectThePlane (Plane plane) const;
        Interval IntersectWithLine (Line line) const;
        friend std::ostream& operator << (std::ostream &os, Triangle& tr);
    };

    struct Plane {
        float A = NAN, B = NAN, C = NAN, D = NAN;

        explicit Plane(Point A1, Point A2, Point A3);
        bool isValid() const;
        Line IntersectionWithOtherPlane(Plane other) const;
        bool IsEqualToOtherPlane (Plane other) const;
    };

    float Determinant(float a11, float a12,
                      float a21, float a22);

    Solution SolveTwoEquations (float a11, float a12, float b1,
                             float a21, float a22, float b2);

    std::ostream& operator << (std::ostream &os, Triangle& tr) {
        os << "A1 (" << tr.A1.x << ", " << tr.A1.y << ", " << tr.A1.z << ")\n"
           << "A2 (" << tr.A2.x << ", " << tr.A2.y << ", " << tr.A2.z << ")\n"
           << "A3 (" << tr.A3.x << ", " << tr.A3.y << ", " << tr.A3.z << ")\n";
        return os;
    }
}