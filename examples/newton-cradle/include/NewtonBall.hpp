#ifndef ATLAS_INCLUDE_ATLAS_TOOLS_NEWTON_BALL_HPP
#define ATLAS_INCLUDE_ATLAS_TOOLS_NEWTON_BALL_HPP

#pragma once

#define NEWTON_BALL_DEFAULT_MASS_WEIGHT 1.0f
#define NEWTON_BALL_DEFAULT_SPRING_ANG 60.0f
#define NEWTON_BALL_DEFAULT_SPRING_KD 0.5f
#define NEWTON_BALL_DEFAULT_SPRING_SL 0.5f
#define NEWTON_BALL_DEFAULT_SPRING_DW 0.7f
#define NEWTON_BALL_DEFAULT_SPRING_L 1.0f
#define NEWTON_BALL_DEFAULT_GRAVITY_FORCE -9.81f

#include "atlas/tools/Tools.hpp"
#include "atlas/utils/Geometry.hpp"
#include "atlas/gl/VertexArrayObject.hpp"
#include "atlas/gl/Buffer.hpp"

namespace math = atlas::math;
namespace utils = atlas::utils;

class NewtonBall final : public utils::Geometry
{
public:
    NewtonBall();
    ~NewtonBall();

    void renderGeometry(atlas::math::Matrix4 const& projection,
        atlas::math::Matrix4 const& view) override;

    // Override this to animate things.
    void updateGeometry(atlas::core::Time<> const& t) override;

private:
    struct mass {
        math::Vector pos;
        math::Vector vt;
        float rate;
        float angle;
        float weight;
        bool collide;
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

    int current;

    float diff;
    // the larger it gets the quicker the newtons ball slows down
    float coff;
};

#endif
