#include "Cloth.hpp"
#include "Paths.hpp"
#include "atlas/gl/Shader.hpp"

namespace math = atlas::math;
namespace utils = atlas::utils;
namespace gl = atlas::gl;

//constexpr auto gridVs =
//            R"(#version 410 core

//layout (location = 0) in vec3 vPosition;

//uniform mat4 projection;
//uniform mat4 view;
//uniform mat4 model;

//void main()
//{
//    gl_Position = projection * view * model * vec4(vPosition, 1.0);
//}
//)";

//        constexpr auto gridFS =
//            R"(#version 410 core

//uniform vec4 colour;

//out vec4 fragColour;

//void main()
//{
//    fragColour = colour;
//}
//)";


Cloth::Cloth() :
    mVertexBuffer(GL_ARRAY_BUFFER),
//    mIndexBuffer(GL_ELEMENT_ARRAY_BUFFER),
//    mRenderMode(1),
//    mNumIndices(150),
//    mNumIndices(15),
    mNumIndices(4),
    vertices(std::vector<math::Vector>(mNumIndices))
{
    mModel = math::Matrix4(1.0f);

//     150
//    for (int i = 0; i < 15; i++)
//    {
//        for (int j = 0; j < 10; j++)
//        {
//            vertices[10 * i + j] = { 0.0f + j + i, 0.0f, 0.0f - j };
//        }
//    }

//    15
//    for (int i = 0; i < 5; i++)
//    {
//        for (int j = 0; j < 3; j++)
//        {
//            vertices[3 * i + j] = { 0.0f + j + i, 0.0f, 0.0f - j };
//        }
//    }

//    4
    vertices[0] = { 0.0f, 0.0f, 0.0f };
    vertices[1] = { 10.0f, 0.0f, -10.0f };
    vertices[2] = { 0.0f, 0.0f, 10.0f };
    vertices[3] = { 10.0f, 0.0f, 0.0f };

//    std::vector<gl::ShaderUnit> shaders
//    {
//        { gridVs, GL_VERTEX_SHADER, true },
//        { gridFS, GL_FRAGMENT_SHADER, true }
//    };

//    mShaders.push_back(gl::Shader(shaders));
//    mShaders[0].compileShaders();
//    mShaders[0].linkShaders();

    mVao.bindVertexArray();
    mVertexBuffer.bindBuffer();
    mVertexBuffer.bufferData(gl::size<math::Vector>(mNumIndices),
        vertices.data(), GL_DYNAMIC_DRAW);
    // try using CLOTHS_LAYOUT_LOCATION
    mVertexBuffer.vertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0,
        gl::bufferOffset<float>(0));
    mVao.enableVertexAttribArray(0);
    mVertexBuffer.unBindBuffer();

//    mIndexBuffer.bindBuffer();
//    mIndexBuffer.bufferData(gl::size<GLuint>(mNumIndices),
//        vertices.data(), GL_DYNAMIC_DRAW);
    mVao.unBindVertexArray();

    // Load the shaders here.
    std::vector<gl::ShaderUnit> shaders
    {
        {std::string(ShaderDirectory) + "Model.vs.glsl", GL_VERTEX_SHADER},
        {std::string(ShaderDirectory) + "Model.fs.glsl", GL_FRAGMENT_SHADER}
    };

    mShaders.push_back(gl::Shader(shaders));
    mShaders[0].setShaderIncludeDir(ShaderDirectory);
    mShaders[0].compileShaders();
    mShaders[0].linkShaders();
    mShaders[0].disableShaders();

    auto var = mShaders[0].getUniformVariable("projection");
    mUniforms.insert(UniformKey("projection", var));
    var = mShaders[0].getUniformVariable("view");
    mUniforms.insert(UniformKey("view", var));
    var = mShaders[0].getUniformVariable("model");
    mUniforms.insert(UniformKey("model", var));
//    var = mShaders[0].getUniformVariable("colour");
//    mUniforms.insert(UniformKey("colour", var));
    var = mShaders[0].getUniformVariable("renderMode");
    mUniforms.insert(UniformKey("renderMode", var));
}

Cloth::~Cloth()
{
    // do destruction here
}

void Cloth::renderGeometry(math::Matrix4 const& projection,
    math::Matrix4 const& view)
{
    if (!mShaders[0].shaderProgramValid())
    {
        return;
    }
    mShaders[0].enableShaders();

    glUniformMatrix4fv(mUniforms["projection"], 1, GL_FALSE,
        &projection[0][0]);
    glUniformMatrix4fv(mUniforms["view"], 1, GL_FALSE, &view[0][0]);
    glUniformMatrix4fv(mUniforms["model"], 1, GL_FALSE, &mModel[0][0]);
//    glUniform1i(mUniforms["renderMode"], mRenderMode);

    glUniform4f(mUniforms["colour"], 0, 0, 0, 1);
    int n = (int)(mNumIndices / 2) - 2;
    glDrawArrays(GL_LINES, n, 4);

    mVao.bindVertexArray();

//    mIndexBuffer.bindBuffer();

    glPointSize(15.0f);
//    float rgb = (64.0f / 255.0f);
//    glUniform4f(mUniforms["colour"], rgb, rgb, rgb, 1);
    glDrawArrays(GL_POINTS, 0, (int)mNumIndices);

//    glUniform1i(mUniforms["renderMode"], 1);
//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//    // avoid hard-code indices
//    glDrawElements(GL_TRIANGLES, 15,
//        GL_UNSIGNED_INT, gl::bufferOffset<float>(0));
//    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

//    mIndexBuffer.unBindBuffer();
    mVao.unBindVertexArray();
    mShaders[0].disableShaders();
}

void Cloth::updateGeometry(atlas::core::Time<> const& t)
{
    vertices[0][0] -= 0.1f;
    vertices[1][0] += 0.1f;

    // update position
    mVao.bindVertexArray();
    mVertexBuffer.bindBuffer();
    mVertexBuffer.bufferData(gl::size<math::Vector>(mNumIndices),
        vertices.data(), GL_DYNAMIC_DRAW);
    mVertexBuffer.unBindBuffer();
    mVao.unBindVertexArray();
}
