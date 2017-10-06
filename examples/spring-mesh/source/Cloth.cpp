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
    mNumIndices(16),
//    mNumIndices(4),
    meshes(mNumIndices)
{
    mModel = math::Matrix4(1.0f);

//     150
//    for (int i = 0; i < 15; i++)
//    {
//        for (int j = 0; j < 10; j++)
//        {
//            meshes[10 * i + j].pos = { 0.0f + j + i, 0.0f, 0.0f - j };
//        }
//    }

    int height = (int) std::sqrt(mNumIndices);
    int width = height;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            meshes[width * i + j].pos = { 0.0f + j + i, 0.0f, 0.0f - j };
            meshes[width * i + j].weight = CLOTH_DEFAULT_MASS_WEIGHT;

            /*
            * define its neighbour
            * Structural springs
            *
            *      0
            *      |
            *    1-x-2
            *      |
            *      3
            *
            */
            // CLOTH_DEFAULT_MASS_NO_NEIGHBOUR
            //std::printf("Index: %d with ", width * i + j);
            //std::printf("[%d, %d] =>", i, j);
            // if there is up
            if (i == 0) {
                //std::printf("no top");
                //std::printf(",");
                meshes[3 * i + j].neighbours[0].index = CLOTH_DEFAULT_MASS_NO_NEIGHBOUR;
            } else {
                // update the index
                // update its spring coefficient
                // update its spring damping force
                // update its spring length
            }
            // if there is buttom
            if (i + 1 == height) {
                //std::printf("no buttom");
                //std::printf(",");
                meshes[3 * i + j].neighbours[1].index = CLOTH_DEFAULT_MASS_NO_NEIGHBOUR;
            } else {
                // update the index
                // update its spring coefficient
                // update its spring damping force
                // update its spring length
            }
            // if there is right
            if (j + 1 == width) {
                //std::printf("no right");
                //std::printf(",");
                meshes[3 * i + j].neighbours[2].index = CLOTH_DEFAULT_MASS_NO_NEIGHBOUR;
            } else {
                // update the index
                // update its spring coefficient
                // update its spring damping force
                // update its spring length
            }
            // if there is left
            if (j == 0) {
                //std::printf("no left");
                //std::printf(",");
                meshes[3 * i + j].neighbours[3].index = CLOTH_DEFAULT_MASS_NO_NEIGHBOUR;
            } else {
                // update the index
                // update its spring coefficient
                // update its spring damping force
                // update its spring length
            }
//            std::printf("\n");
            /* Shear springs
            *
            *    5     6
            *     \   /
            *       x
            *     /   \
            *    7     8
            *
            */
            // if there is top right
            if (i == 0 || j + 1 == width) {
                //std::printf("no top right");
                //std::printf(",");
                meshes[3 * i + j].neighbours[4].index = CLOTH_DEFAULT_MASS_NO_NEIGHBOUR;
            } else {
                // update the index
                // update its spring coefficient
                // update its spring damping force
                // update its spring length
            }
            // if there is top left
            if (i == 0 || j == 0) {
                //std::printf("no top left");
                //std::printf(",");
                meshes[3 * i + j].neighbours[5].index = CLOTH_DEFAULT_MASS_NO_NEIGHBOUR;
            } else {
                // update the index
                // update its spring coefficient
                // update its spring damping force
                // update its spring length
            }
            // if there is buttom right
            if (i + 1 == height || j + 1 == width) {
                //std::printf("no buttom right");
                //std::printf(",");
                meshes[3 * i + j].neighbours[6].index = CLOTH_DEFAULT_MASS_NO_NEIGHBOUR;
            } else {
                // update the index
                // update its spring coefficient
                // update its spring damping force
                // update its spring length
            }
            // if there is buttom left
            if (i + 1 == height || j == 0) {
                //std::printf("no buttom left");
                //std::printf(",");
                meshes[3 * i + j].neighbours[7].index = CLOTH_DEFAULT_MASS_NO_NEIGHBOUR;
            } else {
                // update the index
                // update its spring coefficient
                // update its spring damping force
                // update its spring length
            }
//            std::printf("\n");
            /* Bending springs
            *
            *       9
            *       |
            *       0
            *       |
            *  10-1-x-2-11
            *       |
            *       3
            *       |
            *       12
            *
            */
            // if there is up up
            if (i - 2 < 0) {
                //std::printf("no top top");
                //std::printf(",");
                meshes[3 * i + j].neighbours[8].index = CLOTH_DEFAULT_MASS_NO_NEIGHBOUR;
            } else {
                // update the index
                // update its spring coefficient
                // update its spring damping force
                // update its spring length
            }
            // if there is buttom buttom
            if (i + 2 > height - 1) {
                //std::printf("no buttom buttom");
                //std::printf(",");
                meshes[3 * i + j].neighbours[9].index = CLOTH_DEFAULT_MASS_NO_NEIGHBOUR;
            } else {
                // update the index
                // update its spring coefficient
                // update its spring damping force
                // update its spring length
            }
            // if there is right right
            if (j + 2 > width - 1) {
                //std::printf("no right right");
                //std::printf(",");
                meshes[3 * i + j].neighbours[10].index = CLOTH_DEFAULT_MASS_NO_NEIGHBOUR;
            } else {
                // update the index
                // update its spring coefficient
                // update its spring damping force
                // update its spring length
            }
            // if there is left left
            if (j - 2 < 0) {
                //std::printf("no left left");
                //std::printf(",");
                meshes[3 * i + j].neighbours[11].index = CLOTH_DEFAULT_MASS_NO_NEIGHBOUR;
            } else {
                // update the index
                // update its spring coefficient
                // update its spring damping force
                // update its spring length
            }
            //std::printf("\n");
        }
    }

//    4
//    meshes[0].pos = { 0.0f, 0.0f, 0.0f };
//    meshes[0].weight = CLOTH_DEFAULT_MASS_WEIGHT;
//    meshes[1].pos = { 10.0f, 0.0f, -10.0f };
//    meshes[1].weight = CLOTH_DEFAULT_MASS_WEIGHT;
//    meshes[2].pos = { 0.0f, 0.0f, 10.0f };
//    meshes[2].weight = CLOTH_DEFAULT_MASS_WEIGHT;
//    meshes[3].pos = { 10.0f, 0.0f, 0.0f };
//    meshes[3].weight = CLOTH_DEFAULT_MASS_WEIGHT;

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
    mVertexBuffer.bufferData(sizeof(struct mass) * meshes.size(),
        meshes.data(), GL_DYNAMIC_DRAW);
    // try using CLOTHS_LAYOUT_LOCATION
    mVertexBuffer.vertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(struct mass),
        gl::bufferOffset<float>(0));
    mVao.enableVertexAttribArray(0);
    mVertexBuffer.unBindBuffer();

//    mIndexBuffer.bindBuffer();
//    mIndexBuffer.bufferData(gl::size<GLuint>(mNumIndices),
//        meshes.data(), GL_DYNAMIC_DRAW);
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
//    meshes[0].pos[0] -= 0.1f;
//    meshes[1].pos[2] += 0.1f;

    // update position
    mVao.bindVertexArray();
    mVertexBuffer.bindBuffer();
    mVertexBuffer.bufferData(sizeof(struct mass) * meshes.size(),
        meshes.data(), GL_DYNAMIC_DRAW);
    mVertexBuffer.unBindBuffer();
    mVao.unBindVertexArray();
}
