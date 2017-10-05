#ifndef ATLAS_INCLUDE_ATLAS_TOOLS_CLOTH_HPP
#define ATLAS_INCLUDE_ATLAS_TOOLS_CLOTH_HPP

#pragma once

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
    atlas::gl::VertexArrayObject mVao;
    atlas::gl::Buffer mVertexBuffer;
//    atlas::gl::Buffer mIndexBuffer;
    std::size_t mNumIndices;
//    int mRenderMode;
    std::vector<math::Vector> vertices;
};

#endif
