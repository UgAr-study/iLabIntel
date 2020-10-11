#pragma once

#include <unordered_map>
#include <utility>
#include <vector>
#include "GeometryHeader.h"

enum {MAXDEPTH = 3, NUMINCUBE = 2};

namespace Alg {

    struct Cube;
    struct node_t;
    struct Octree;

    using res_table = std::unordered_map<Geom::Triangle*, Geom::Triangle>;
    using triangle_vec = std::vector<Geom::Triangle>;
    using triangles_ptr = std::vector<Geom::Triangle*>;
    using special_trs = std::unordered_map<Geom::Triangle*, Cube*>;

    struct Cube {
        Geom::Point left_bot, right_top;
        Cube* parent_cube;
        unsigned number_of_elems = 0;
        triangles_ptr triangles;

        Cube () : left_bot(), right_top() {}
        explicit Cube(Geom::Point leftBot, Geom::Point rightTop ,
                      triangles_ptr& triangle, special_trs & spec_trs, Cube* parent);
        signed char IsTriangleInCube (Geom::Triangle triangle) const;
        void CheckEveryTriangleInCube (res_table& ans_table);
        void CheckThisTriangle (Geom::Triangle *triangle, res_table& ans_table);
        Cube* CubeFraction (special_trs& spec_trs);
    };

    struct node_t {
        Cube cube_;
        node_t *_child[8] = {nullptr};
        explicit node_t(Cube cb) { cube_ = cb; }
    };

    struct Octree {
        node_t *_top;
        unsigned maxDepth, max_num_of_elems_in_cube;
        special_trs spec_trs;
        explicit Octree(unsigned MaxDepth, unsigned num_elem_in_cube, triangle_vec& triangles);
        ~Octree();
        Octree& operator= (const Octree& t) = delete;
        Octree(const Octree& t) = delete;
    private:
        node_t* BuildTree (triangle_vec& triangles);
        void RecursiveBuild (node_t* top, unsigned level);
        void RecursiveDelete(node_t *top);
    };

    std::vector<unsigned> FindIntersections (triangle_vec& triangles);
    void RecursiveDescent (node_t *top, res_table& ans_table);
    bool DefineCube(const triangle_vec& triangles, Geom::Point *dots);
}
