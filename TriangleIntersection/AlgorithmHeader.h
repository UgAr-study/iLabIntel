#pragma once

#include <unordered_map>
#include <utility>
#include <vector>
#include <random>
#include "GeometryHeader.h"

enum {MAXDEPTH = 3, NUMINCUBE = 10};

namespace Alg {

    struct Cube;
    struct node_t;
    struct Octree;

    using res_table = std::unordered_map<Geom::Triangle*, Geom::Triangle>;
    using triangle_vec = std::vector<Geom::Triangle>;
    using triangles_ptr = std::vector<Geom::Triangle*>;
    using special_trs = std::unordered_map<Geom::Triangle*, Cube*>;

    struct Cube {
    private:
        Geom::Point left_bot, right_top;
        unsigned number_of_elems = 0;
        Cube* parent_cube;
        triangles_ptr triangles;

    public:
        unsigned getNumberOfElems () const { return number_of_elems; }

        Cube () : left_bot(), right_top() { parent_cube = nullptr; }
        explicit Cube(Geom::Point leftBot, Geom::Point rightTop ,
                      triangles_ptr& triangle, special_trs & spec_trs, Cube* parent);
        signed char IsTriangleInCube (Geom::Triangle triangle) const;
        void CheckEveryTriangleInCube (res_table& ans_table) const;
        void CheckThisTriangle (Geom::Triangle *triangle, res_table& ans_table) const;
        Cube* CubeFraction (special_trs& spec_trs);
    };

    struct node_t {
        explicit node_t(Cube cb) { cube_ = std::move(cb); }
        Cube cube_;
        node_t *_child[8] = {nullptr};
    };

    struct Octree {
    public:
        explicit Octree(unsigned MaxDepth, unsigned num_elem_in_cube, triangle_vec& triangles);
        ~Octree();
        Octree& operator= (const Octree& t) = delete;
        Octree(const Octree& t) = delete;

        void RecursiveDescent (res_table& ans_table);
        void CheckSpecialTriangles (res_table& ans_table);

    private:
        node_t *_top;
        unsigned maxDepth, max_num_of_elems_in_cube;
        special_trs spec_trs;

        node_t* BuildTree (triangle_vec& triangles);
        void RecursiveBuild (node_t* top, unsigned level);
        void RecursiveDescent (node_t *top, res_table& ans_table);
        void RecursiveDelete(node_t *top);
    };

    std::vector<unsigned> FindIntersections (triangle_vec& triangles);

    bool DefineCube(const triangle_vec& triangles, Geom::Point *dots);
}
