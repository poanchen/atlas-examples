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
    fg(math::Vector(0, CLOTH_DEFAULT_GRAVITY_FORCE, 0)),
//    mIndexBuffer(GL_ELEMENT_ARRAY_BUFFER),
//    mRenderMode(1),
    mNumIndices(400),
//    mNumIndices(16),
//    mNumIndices(4),
    mMeshes(mNumIndices)
{
    mModel = math::Matrix4(1.0f);

//     150
//    for (int i = 0; i < 15; i++)
//    {
//        for (int j = 0; j < 10; j++)
//        {
//            mMeshes[10 * i + j].pos = { 0.0f + j + i, 0.0f, 0.0f - j };
//        }
//    }

    int height = (int) std::sqrt(mNumIndices);
    int width = height;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            mMeshes[width * i + j].pos = { 0.0f + j + i, 0.0f, 0.0f - j }; // initial position
            mMeshes[width * i + j].vt = { 1.0f, 1.0f, 1.0f }; // initial velocity
            mMeshes[width * i + j].weight = CLOTH_DEFAULT_MASS_WEIGHT;

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
                mMeshes[width * i + j].neighbours[0].index = CLOTH_DEFAULT_MASS_NO_NEIGHBOUR;
            } else {
                mMeshes[width * i + j].neighbours[0] = { width * i + j - width, { CLOTH_DEFAULT_SPRING_SKSS, CLOTH_DEFAULT_SPRING_KD, CLOTH_DEFAULT_SPRING_SL } };
            }
            // if there is buttom
            if (i + 1 == height) {
                //std::printf("no buttom");
                //std::printf(",");
                mMeshes[width * i + j].neighbours[1].index = CLOTH_DEFAULT_MASS_NO_NEIGHBOUR;
            } else {
                mMeshes[width * i + j].neighbours[1] = { width * i + j + width, { CLOTH_DEFAULT_SPRING_SKSS, CLOTH_DEFAULT_SPRING_KD, CLOTH_DEFAULT_SPRING_SL } };
            }
            // if there is right
            if (j + 1 == width) {
                //std::printf("no right");
                //std::printf(",");
                mMeshes[width * i + j].neighbours[2].index = CLOTH_DEFAULT_MASS_NO_NEIGHBOUR;
            } else {
                mMeshes[width * i + j].neighbours[2] = { width * i + j + 1, { CLOTH_DEFAULT_SPRING_SKSS, CLOTH_DEFAULT_SPRING_KD, CLOTH_DEFAULT_SPRING_SL } };
            }
            // if there is left
            if (j == 0) {
                //std::printf("no left");
                //std::printf(",");
                mMeshes[width * i + j].neighbours[3].index = CLOTH_DEFAULT_MASS_NO_NEIGHBOUR;
            } else {
                mMeshes[width * i + j].neighbours[3] = { width * i + j - 1, { CLOTH_DEFAULT_SPRING_SKSS, CLOTH_DEFAULT_SPRING_KD, CLOTH_DEFAULT_SPRING_SL } };
            }
            //std::printf("\n");
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
                mMeshes[width * i + j].neighbours[4].index = CLOTH_DEFAULT_MASS_NO_NEIGHBOUR;
            } else {
                mMeshes[width * i + j].neighbours[4] = { width * i + j - width + 1, { CLOTH_DEFAULT_SPRING_SKSD, CLOTH_DEFAULT_SPRING_KD, CLOTH_DEFAULT_SPRING_DW } };
            }
            // if there is top left
            if (i == 0 || j == 0) {
                //std::printf("no top left");
                //std::printf(",");
                mMeshes[width * i + j].neighbours[5].index = CLOTH_DEFAULT_MASS_NO_NEIGHBOUR;
            } else {
                mMeshes[width * i + j].neighbours[5] = { width * i + j - width - 1, { CLOTH_DEFAULT_SPRING_SKSD, CLOTH_DEFAULT_SPRING_KD, CLOTH_DEFAULT_SPRING_DW } };
            }
            // if there is buttom right
            if (i + 1 == height || j + 1 == width) {
                //std::printf("no buttom right");
                //std::printf(",");
                mMeshes[width * i + j].neighbours[6].index = CLOTH_DEFAULT_MASS_NO_NEIGHBOUR;
            } else {
                mMeshes[width * i + j].neighbours[6] = { width * i + j + width + 1, { CLOTH_DEFAULT_SPRING_SKSD, CLOTH_DEFAULT_SPRING_KD, CLOTH_DEFAULT_SPRING_DW } };
            }
            // if there is buttom left
            if (i + 1 == height || j == 0) {
                //std::printf("no buttom left");
                //std::printf(",");
                mMeshes[width * i + j].neighbours[7].index = CLOTH_DEFAULT_MASS_NO_NEIGHBOUR;
            } else {
                mMeshes[width * i + j].neighbours[7] = { width * i + j + width - 1, { CLOTH_DEFAULT_SPRING_SKSD, CLOTH_DEFAULT_SPRING_KD, CLOTH_DEFAULT_SPRING_DW } };
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
                mMeshes[width * i + j].neighbours[8].index = CLOTH_DEFAULT_MASS_NO_NEIGHBOUR;
            } else {
                mMeshes[width * i + j].neighbours[8] = { width * i + j - width * 2, { CLOTH_DEFAULT_SPRING_SKSB, CLOTH_DEFAULT_SPRING_KD, CLOTH_DEFAULT_SPRING_SL * 2 } };
            }
            // if there is buttom buttom
            if (i + 2 > height - 1) {
                //std::printf("no buttom buttom");
                //std::printf(",");
                mMeshes[width * i + j].neighbours[9].index = CLOTH_DEFAULT_MASS_NO_NEIGHBOUR;
            } else {
                mMeshes[width * i + j].neighbours[9] = { width * i + j + width * 2, { CLOTH_DEFAULT_SPRING_SKSB, CLOTH_DEFAULT_SPRING_KD, CLOTH_DEFAULT_SPRING_SL * 2 } };
            }
            // if there is right right
            if (j + 2 > width - 1) {
                //std::printf("no right right");
                //std::printf(",");
                mMeshes[width * i + j].neighbours[10].index = CLOTH_DEFAULT_MASS_NO_NEIGHBOUR;
            } else {
                mMeshes[width * i + j].neighbours[10] = { width * i + j + 2, { CLOTH_DEFAULT_SPRING_SKSB, CLOTH_DEFAULT_SPRING_KD, CLOTH_DEFAULT_SPRING_SL * 2 } };
            }
            // if there is left left
            if (j - 2 < 0) {
                //std::printf("no left left");
                //std::printf(",");
                mMeshes[width * i + j].neighbours[11].index = CLOTH_DEFAULT_MASS_NO_NEIGHBOUR;
            } else {
                mMeshes[width * i + j].neighbours[11] = { width * i + j - 2, { CLOTH_DEFAULT_SPRING_SKSB, CLOTH_DEFAULT_SPRING_KD, CLOTH_DEFAULT_SPRING_SL * 2 } };
            }
            //std::printf("\n");
        }
    }

    //debug
//    for (int i = 0; i < height; i++)
//    {
//        for (int j = 0; j < width; j++)
//        {
//            std::printf("Index: %d with ", width * i + j);
//            std::printf("[%d, %d] => ", i, j);
//            std::printf("weight: %f | ", mMeshes[width * i + j].weight);
//            for (int k = 0; k < 12; k++)
//            {
//                switch (k) {
//                case 0:
//                    std::printf("top: %d, ", mMeshes[width * i + j].neighbours[k].index);
//                    break;
//                case 1:
//                    std::printf("buttom: %d, ", mMeshes[width * i + j].neighbours[k].index);
//                    break;
//                case 2:
//                    std::printf("right: %d, ", mMeshes[width * i + j].neighbours[k].index);
//                    break;
//                case 3:
//                    std::printf("left: %d, ", mMeshes[width * i + j].neighbours[k].index);
//                    break;
//                case 4:
//                    std::printf("top right: %d, ", mMeshes[width * i + j].neighbours[k].index);
//                    break;
//                case 5:
//                    std::printf("top left: %d, ", mMeshes[width * i + j].neighbours[k].index);
//                    break;
//                case 6:
//                    std::printf("buttom right: %d, ", mMeshes[width * i + j].neighbours[k].index);
//                    break;
//                case 7:
//                    std::printf("buttom left: %d, ", mMeshes[width * i + j].neighbours[k].index);
//                    break;
//                case 8:
//                    std::printf("top top: %d, ", mMeshes[width * i + j].neighbours[k].index);
//                    break;
//                case 9:
//                    std::printf("buttom buttom: %d, ", mMeshes[width * i + j].neighbours[k].index);
//                    break;
//                case 10:
//                    std::printf("right right: %d, ", mMeshes[width * i + j].neighbours[k].index);
//                    break;
//                case 11:
//                    std::printf("left left: %d, ", mMeshes[width * i + j].neighbours[k].index);
//                    break;
//                default:
//                    break;
//                }
//            }
//            std::printf("\n");
//        }
//    }

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
    mVertexBuffer.bufferData(sizeof(struct mass) * mMeshes.size(),
        mMeshes.data(), GL_DYNAMIC_DRAW);
    // try using CLOTHS_LAYOUT_LOCATION
    mVertexBuffer.vertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(struct mass),
        gl::bufferOffset<float>(0));
    mVao.enableVertexAttribArray(0);
    mVertexBuffer.unBindBuffer();

//    mIndexBuffer.bindBuffer();
//    mIndexBuffer.bufferData(gl::size<GLuint>(mNumIndices),
//        mMeshes.data(), GL_DYNAMIC_DRAW);
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

    glPointSize(10.0f);
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
    std::vector<mass> meshes = mMeshes;
//    int width = (int) std::sqrt(mNumIndices);
    for (int i = 0; i < mNumIndices; i++)
    {
        // make sure we do not update the fixed point
        switch(CLOTH_DEFAULT_KIND)
        {
        case 0:
            // second row fixed
            if ((i > 19 && i < 39)) {
                continue;
            }
            break;
        case 1:
            // left five of second row and right five of second row fixed
            if ((i > 19 && i < 25) || (i > 34 && i < 40)) {
                continue;
            }
            break;
        case 2:
            // second row and last second row fixed
            if ((i > 19 && i < 39) || (i > 359 && i < 379)) {
                continue;
            }
            break;
        case 3:
            // last second row fixed
            if (i > 359 && i < 379) {
                continue;
            }
            break;
        case 4:
            // middle row fixed
            if (i > 200 && i < 219) {
                continue;
            }
            break;
        case 5:
            // five middle point in middle row fixed
            if (i > 204 && i < 210) {
                continue;
            }
            break;
        case 6:
            // free fall
            break;
        }

//        std::printf("Index %d: ", i);

        // Compute the sum of kd from all its neighbours
        float sumKd = 0.0f;
        for (int j = 0; j < 12; j++)
        {
            // we simply ignore all the neighbours that does not exist
            if (meshes[i].neighbours[j].index == CLOTH_DEFAULT_MASS_NO_NEIGHBOUR) {
                continue;
            }

            sumKd += meshes[i].neighbours[j].kd;
        }

        // Compute the damping force fd
        fd = -sumKd * meshes[i].vt;

        // Compute the total force
        f = fg + fd;

        // Compute the sum of spring force fs from all its neighbours
        for (int j = 0; j < 12; j++)
        {
            // we simply ignore all the neighbours that does not exist
            if (meshes[i].neighbours[j].index == CLOTH_DEFAULT_MASS_NO_NEIGHBOUR) {
                continue;
            }

//            std::printf("%d, ", meshes[i].neighbours[j].index);

            // Compute the current spring length
            const struct mass currentParent = meshes[meshes[i].neighbours[j].index];
            slt = glm::length(currentParent.pos - meshes[i].pos);
            f += math::Vector(-currentParent.neighbours[j].sks * (meshes[i].neighbours[j].sl - slt) * glm::normalize(currentParent.pos - meshes[i].pos));
        }

        math::Vector a = f / meshes[i].weight;
        math::Vector v = meshes[i].vt * t.deltaTime + 0.5f * a * t.deltaTime * t.deltaTime;

        // Update each mass velocity
        meshes[i].vt = meshes[i].vt + a * t.deltaTime;

        // Update each mass position
        meshes[i].pos = meshes[i].pos + v;


//        std::printf("sumKd: %f, ", sumKd);
//        std::printf("fd: [%f, %f, %f], ", fd[0], fd[1], fd[2]);
//        std::printf("meshes[%d].pos: [%f, %f, %f], ", i, meshes[i].pos[0], meshes[i].pos[1], meshes[i].pos[2]);
//        std::printf("meshes[%d].vt: [%f, %f, %f]", i, meshes[i].vt[0], meshes[i].vt[1], meshes[i].vt[2]);
//        std::printf("\n");
    }

    mMeshes = meshes;

    // update position
    mVao.bindVertexArray();
    mVertexBuffer.bindBuffer();
    mVertexBuffer.bufferData(sizeof(struct mass) * mMeshes.size(),
        mMeshes.data(), GL_DYNAMIC_DRAW);
    mVertexBuffer.unBindBuffer();
    mVao.unBindVertexArray();
}
