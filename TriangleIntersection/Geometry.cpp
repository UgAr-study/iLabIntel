#include "GeometryHeader.h"




//////////////////////
////////Point/////////
//////////////////////


Geom::Point Geom::Point::operator/(float a) const {
    Point res;
    res.x = x / a;
    res.y = y / a;
    res.z = z / a;
    return res;
}

Geom::Point Geom::Point::operator-(Geom::Point &p) const {
    Point res;
    res.x = x - p.x;
    res.y = y - p.y;
    res.z = z - p.z;
    return res;
}

Geom::Point Geom::Point::operator+(Geom::Point &p) const {
    Point res;
    res.x = x + p.x;
    res.y = y + p.y;
    res.z = z + p.z;
    return res;
}

Geom::Point Geom::Point::operator*(float a) const {
    Point res;
    res.x = x * a;
    res.y = y * a;
    res.z = z * a;
    return res;
}


//////////////////////
////////Vector////////
//////////////////////

bool Geom::Vector::IsCollinearToOther(Vector other) const {
    /*Vector v_this = Normalize();
    Vector v_other = other.Normalize();
    if ((v_this.VectorMult(v_other)).IsZero())
        return true;*/
    if ((VectorMult(other)).IsZero())
        return true;
    return false;
}

Geom::Vector Geom::Vector::VectorMult(Vector second_vec) const {
    Vector res;

    res.V.x = Determinant(V.y, V.z,
                          second_vec.V.y, second_vec.V.z);
    if (std::abs(res.V.x) <= PRECISION)
        res.V.x = 0;

    res.V.y = -Determinant(V.x, V.z,
                           second_vec.V.x, second_vec.V.z);
    if (std::abs(res.V.y) <= PRECISION)
        res.V.y = 0;

    res.V.z = Determinant(V.x, V.y,
                          second_vec.V.x, second_vec.V.y);
    if (std::abs(res.V.z) <= PRECISION)
        res.V.z = 0;

    return res;
}

bool Geom::Vector::IsZero() const {
    if (V.Abs() <= PRECISION)
        return true;
    return false;
}

float Geom::Vector::ScalarMult(Geom::Vector second_vec) const {
    float scalar = V.x * second_vec.V.x + V.y * second_vec.V.y + V.z * second_vec.V.z;
    return (std::abs(scalar) <= PRECISION) ? 0 : scalar;
}

bool Geom::Vector::IsPerpendicularToOther(Vector other) const {
    if (!isValid() || !other.isValid())
        return false;
    if (std::abs (ScalarMult(other)) <= PRECISION)
        return true;
    return false;
}

Geom::Vector Geom::Vector::Normalize() const {
    Vector v {Point{0, 0, 0}, V / Abs()};
    return v;
}

//////////////////////
///////Interval///////
//////////////////////

bool Geom::Interval::IsPointBelongsToInterval(Point M) const {
    if (!M.isValid())
        return false;

    Vector v1 {C1, M}, v2 {C2, M};
    if (v1.IsCollinearToOther(v2) && v1.ScalarMult(v2) <= PRECISION)
        return true;
    return false;
}

bool Geom::Interval::IsOverlapWithOther(Interval other) const {
    if (!other.isValid() || !isValid())
        return false;

    Vector v1 {C1, C2},
           v2 {other.C1, other.C2};
    if (!v1.IsCollinearToOther(v2))
        return false;

    if (IsPointBelongsToInterval(other.C1) || IsPointBelongsToInterval(other.C2) ||
        other.IsPointBelongsToInterval(C1) || other.IsPointBelongsToInterval(C2))
        return true;

    return false;
}

bool Geom::Interval::IsIntersectWithOther(Interval other) const {
    Line line1 {C1, C2}, line2 {other.C1, other.C2};
    bool coinc;
    Point in_p = line1.IntersectionWithOtherLine(line2, &coinc);
    if (in_p.isValid())
        if (IsPointBelongsToInterval(in_p) && other.IsPointBelongsToInterval(in_p))
            return true;
    return false;
}

//////////////////////
/////////Line/////////
//////////////////////

Geom::Point Geom::Line::IntersectionWithOtherLine(Geom::Line other, bool *isCoinsedent) const {
    Point res; //P1 + V1 * a1 = P2 + V2 * a2 | * V2 -> a1 = ... -> find res
    Vector v12 = vec.VectorMult(other.vec), p{M, other.M};
    Vector vp = p.VectorMult(other.vec);

    if (v12.IsZero()) {
        *isCoinsedent = false;
        if (Vector{M, other.M}.IsCollinearToOther(vec))
            *isCoinsedent = true;
        return res;
    }

    float a = vp.Abs() / v12.Abs();
    if (vp.ScalarMult(v12) <= PRECISION)
        a = -a;

    res.x = M.x + a * vec.V.x;
    res.y = M.y + a * vec.V.y;
    res.z = M.z + a * vec.V.z;
    return res;
}

//////////////////////
///////Triangle///////
//////////////////////

void Geom::Triangle::Dump(std::ostream &os) const {
    bool valid = isValid();
    os << "Triangle № " << number << " [valid = " << valid << "]"
        << "\n\t" << A1.x << " " << A1.y << " " << A1.z << "\n"
        << "\t" << A2.x << " " << A2.y << " " << A2.z << "\n"
        << "\t" << A3.x << " " << A3.y << " " << A3.z << "\n";
}

std::ostream& operator << (std::ostream &os, Geom::Triangle& tr) {
    tr.Dump(os);
    return os;
}

bool Geom::Triangle::IsIntersectWithOther(Triangle other) const {

    Plane pl1{A1, A2, A3}, pl2{other.A1, other.A2, other.A3};
    if (!IsIntersectThePlane(pl2) || !other.IsIntersectThePlane(pl1))
        return false;

    Line interln = pl1.IntersectionWithOtherPlane(pl2);
    if (!interln.isValid())
        return false;

    if (interln.vec.IsZero()){
        std::vector<Interval> intervals {Interval{A1, A2}, Interval{A2, A3}, Interval{A1, A3},
                                         Interval{other.A1, other.A2}, Interval{other.A2, other.A3}, Interval{other.A1, other.A3}};

        for (int i = 0; i < 3; ++i) {
            for (int j = 3; j < 6; ++j) {
                if (intervals[i].IsIntersectWithOther(intervals[j])) {
                    return true;
                }
            }
        }

        return false;
    }

    Interval intersect_interval1 = IntersectWithLine(interln),
             intersect_interval2 = other.IntersectWithLine(interln); //???

    if (intersect_interval1.IsOverlapWithOther(intersect_interval2))
        return true;

    return false;
}

bool Geom::Triangle::isValid() const {
    Vector v1{A1, A2}, v2{A1, A3};
    if(!v1.IsCollinearToOther(v2))
        return true;
    return false;
}

bool Geom::Triangle::IsIntersectThePlane(Geom::Plane plane) const {
    if (!plane.isValid())
        return false;
    float sign1, sign2, sign3;
    sign1 = plane.A * A1.x + plane.B * A1.y + plane.C * A1.z + plane.D;
    sign2 = plane.A * A2.x + plane.B * A2.y + plane.C * A2.z + plane.D;
    sign3 = plane.A * A3.x + plane.B * A3.y + plane.C * A3.z + plane.D;

    if (sign1 * sign2 <= PRECISION ||
        sign2 * sign3 <= PRECISION ||
        sign1 * sign3 <= PRECISION)
        return true;

    return false;
}

Geom::Interval Geom::Triangle::IntersectWithLine(Line line) const {
    Interval interval1 {A1, A2}, interval2 {A2, A3}, interval3 {A1, A3}, res;
    Line line1 {A1, A2}, line2 {A2, A3}, line3 {A1, A3};
    bool coincident1 = false, coincident2 = false, coincident3 = false;
    Point p_i1 = line.IntersectionWithOtherLine(line1, &coincident1),
          p_i2 = line.IntersectionWithOtherLine(line2, &coincident2),
          p_i3 = line.IntersectionWithOtherLine(line3, &coincident3);

    if (!p_i1.isValid() && coincident1)
        return interval1;
    if (!p_i2.isValid() && coincident2)
        return interval2;
    if (!p_i3.isValid() && coincident3)
        return interval3;

    if (interval1.IsPointBelongsToInterval(p_i1)) {
        res.C1 = p_i1;
        if (interval2.IsPointBelongsToInterval(p_i2))
            res.C2 = p_i2;
        else
            res.C2 = p_i3;

        if (res.Length() >= PRECISION)
            return res;
    }

    if (interval2.IsPointBelongsToInterval(p_i2)) {
        res.C1 = p_i2;
        res.C2 = p_i3;
    }

    return res;
}

//////////////////////
////////Plane/////////
//////////////////////

Geom::Plane::Plane(Point A1, Point A2, Point A3) {
    Triangle tr(0, A1, A2, A3);
    if (!tr.isValid())
        return;
    A = Determinant(A3.y - A1.y, A3.z - A1.z,
                    A2.y - A1.y, A2.z - A1.z);
    B = -Determinant(A3.x - A1.x, A3.z - A1.z,
                    A2.x - A1.x, A2.z - A1.z);
    C = Determinant(A3.x - A1.x, A3.y - A1.y,
                    A2.x - A1.x, A2.y - A1.y);
    D = -A1.x * A - A1.y * B - A1.z * C;

    Point p{A, B, C}, p0{0, 0, 0};
    Vector v{p0, p};
    Vector normal_v = v.Normalize();
    A = normal_v.V.x;
    B = normal_v.V.y;
    C = normal_v.V.z;
    D = D / v.Abs();

}

Geom::Line Geom::Plane::IntersectionWithOtherPlane(Plane other) const {
    if (IsEqualToOtherPlane(other))
        return Line{Point{0, 0, 0}, Vector{0, 0, 0}};

    Line res;
    Vector v1{A, B, C}, v2{other.A, other.B, other.C};
    res.vec = v1.VectorMult(v2);

    if (res.vec.IsZero() || !isValid() || !other.isValid())
        return Line{};

    float a = (-other.D * v1.ScalarMult(v2) + D * v2.Abs() * v2.Abs()) /
              (v1.ScalarMult(v2) * v1.ScalarMult(v2) - v2.Abs() * v2.Abs() * v1.Abs() * v1.Abs());
    float b = (-D * v1.ScalarMult(v2) + other.D * v1.Abs() * v1.Abs()) /
              (v1.ScalarMult(v2) * v1.ScalarMult(v2) - v2.Abs() * v2.Abs() * v1.Abs() * v1.Abs());

    res.M.x = a * A + b * other.A;
    res.M.y = a * B + b * other.B;
    res.M.z = a * C + b * other.C;

    return res;
}

bool Geom::Plane::IsEqualToOtherPlane(Geom::Plane other) const {
    if (!isValid() || !other.isValid())
        return false;
    Vector v1{A, B, C}, v2{other.A, other.B, other.C};
    if (v1.VectorMult(v2).IsZero() && D * other.A - A * other.D <= PRECISION)
        return true;
    return false;
}

Geom::Point Geom::Plane::IntersectionWithLine(Geom::Line line) const {
    Vector n {A, B, C};
    if (n.IsPerpendicularToOther(line.vec))
        return {};
    float t = -(A * line.M.x + B * line.M.y + C * line.M.z + D) /
               (A * line.vec.V.x + B * line.vec.V.y + C * line.vec.V.z);
    Point p = line.vec.V * t;
    Point res = line.M + p;
    return res;
}

void Geom::Plane::Dump(std::ostream &os) const {
    os << "Plane: valid = " << isValid() << std::endl
        << "\tA: " << A << "\n\tB: " << B << "\n\tC: " << C << std::endl;
}

std::ostream& operator << (std::ostream &os, Geom::Plane& plane) {
    plane.Dump(os);
    return os;
}








//////////////////////
/////HelpFunctions////
//////////////////////

Geom::Solution Geom::SolveTwoEquations(float a11, float a12, float b1,
                                    float a21, float a22,  float b2) {
    Solution res;
    float main_det = Determinant(a11, a12,
                                 a21, a22);

    std::cout << main_det << std::endl;
    if (main_det >= PRECISION) {
        float det1 = Determinant(b1, a12,
                                 b2, a22);
        float det2 = Determinant(a11, b1,
                                 a21, b2);

        res.x1 = det1 / main_det;
        res.x2 = det2 / main_det;

        return res;
    }
    return res;
}

float Geom::Determinant(float a11, float a12,
                  float a21, float a22) {
    return a11 * a22 - a12 * a21;
}