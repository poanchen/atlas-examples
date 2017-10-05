#ifndef ATLAS_INCLUDE_ATLAS_TOOLS_CLOTH_HPP
#define ATLAS_INCLUDE_ATLAS_TOOLS_CLOTH_HPP

#pragma once

#define CLOTH_DEFAULT_MASS_WEIGHT 0.2f;
#define CLOTH_DEFAULT_MASS_NO_NEIGHBOUR -1;

#include "atlas/tools/Tools.hpp"
#include "atlas/utils/Geometry.hpp"
#include "atlas/gl/VertexArrayObject.hpp"
#include "atlas/gl/Buffer.hpp"

namespace math = atlas::math;
namespace utils = atlas::utils;

class Cloth final : public utils::Geometry
{
public:
    Cloth();
    ~Cloth();

    void renderGeometry(atlas::math::Matrix4 const& projection,
        atlas::math::Matrix4 const& view) override;

    // Override this to animate things.
    void updateGeometry(atlas::core::Time<> const& t) override;

private:
    struct neighbour {
        int index; // when index is -1, that means it does not have neighbour for that position
        struct {
            float sks; // spring coefficient
            float kd; // spring damping force
            float sl; // spring length
        };
    };

    struct mass {
        math::Vector pos;
        float weight;
        struct neighbour neighbours[12];
    };

private:
    atlas::gl::VertexArrayObject mVao;
    atlas::gl::Buffer mVertexBuffer;
//    atlas::gl::Buffer mIndexBuffer;
    std::size_t mNumIndices; // assume it can be made up as a square
//    int mRenderMode;
    std::vector<mass> meshes;
};

#endif
