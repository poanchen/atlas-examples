#ifndef ATLAS_INCLUDE_ATLAS_TOOLS_CLOTH_HPP
#define ATLAS_INCLUDE_ATLAS_TOOLS_CLOTH_HPP

#pragma once

#define CLOTH_DEFAULT_MASS_WEIGHT 2.0f
#define CLOTH_DEFAULT_MASS_NO_NEIGHBOUR -1
#define CLOTH_DEFAULT_SPRING_SKSS 40.0f
#define CLOTH_DEFAULT_SPRING_SKSD 35.0f
#define CLOTH_DEFAULT_SPRING_SKSB 15.0f
#define CLOTH_DEFAULT_SPRING_KD 0.5f
#define CLOTH_DEFAULT_SPRING_SL 0.5f
#define CLOTH_DEFAULT_SPRING_DW 0.7f
#define CLOTH_DEFAULT_GRAVITY_FORCE -9.087f

// 0: second row fixed
// 1: left five of second row and right five of second row fixed
// 2: second row and last second row fixed
// 3: last second row fixed
// 4: middle row fixed
// 5: five middle point in middle row fixed
// 6: free fall
#define CLOTH_DEFAULT_KIND 0

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
        math::Vector vt;
        float weight;
        struct neighbour neighbours[12];
    };

private:
    atlas::gl::VertexArrayObject mVao;
    atlas::gl::Buffer mVertexBuffer;
//    atlas::gl::Buffer mIndexBuffer;
    std::size_t mNumIndices; // assume it can be made up as a square
//    int mRenderMode;
    std::vector<mass> mMeshes;

    float slt; // spring length

    // Define the damping force
    math::Vector fd;

    // Gravity force
    math::Vector fg; // gravity to the ground

    // Total force
    math::Vector f;
};

#endif
