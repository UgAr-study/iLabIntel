#include "../GeometryHeader.h"
#include <gtest/gtest.h>

TEST (Point, Abs) {

}

TEST (Vector, IsCollinear) {
    Geom::Vector v1{0.5, 0.5, 0.5}, v2{2, 2, 2};
    ASSERT_TRUE(v1.IsCollinearToOther(v2));
}

TEST (Vector, IsZero) {
    Geom::Vector v{0.00001, 0.00001, 0.00001};
    ASSERT_TRUE(!v.IsZero());
}

TEST (Vector, IsValid) {
    Geom::Vector v;
    ASSERT_TRUE(!v.isValid());
}

TEST (Vector, VectorMult) {
    Geom::Vector v1{1, 1, 1}, v2{2, 3, 4}, v_m;
    v_m = v1.VectorMult(v2);
    ASSERT_TRUE(v_m.V.x == 1 && v_m.V.y == -2 && v_m.V.z == 1);
}
/*
TEST (Vector, ScalarMult) {

}

TEST (Vector, Abs) {

}

TEST (Interval, IsPointBelongs) {

}

TEST (Interval, Intersect) {

}

TEST (Interval, Overlap) {

}


TEST (Line, IsValid) {

}
*/

TEST (Line, Intersect) {
    Geom::Point p1{0, 0, 0}, p2{1, 1, 1}, p_int;
    Geom::Vector v1{1, 1, 0}, v2{0, 0, 1};
    Geom::Line l1{p1, v1}, l2{p2, v2};
    p_int = l1.IntersectionWithOtherLine(l2);
    ASSERT_TRUE((std::abs(p_int.x - 1) <= Geom::PRECISION) &&
                        (std::abs(p_int.y - 1) <= Geom::PRECISION) &&
                        (std::abs(p_int.z) <= Geom::PRECISION));
}
/*
TEST (Triangle, IsValid) {

}

TEST (Triangle, IntersectPlane) {

}

TEST (Triangle, IntersectLine) {

}

TEST (Triangle, IntersectTriangle) {

}

TEST (Plane, IsValid) {

}

TEST (Plane, Intersect) {

}

TEST (Plane, IsEqual) {

}

TEST(HelpF, Determinant) {

}

TEST (HelpF, SolveEq) {

}
*/


int main () {
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}

