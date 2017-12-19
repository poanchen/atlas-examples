#include "NewtonBall.hpp"
#include "Paths.hpp"
#include "atlas/gl/Shader.hpp"

namespace math = atlas::math;
namespace utils = atlas::utils;
namespace gl = atlas::gl;

NewtonBall::NewtonBall() :
    mVertexBuffer(GL_ARRAY_BUFFER),
    fg(math::Vector(0, NEWTON_BALL_DEFAULT_GRAVITY_FORCE, 0)),
    slt(4.0f),
    // slt(NEWTON_BALL_DEFAULT_SPRING_L),
//    mIndexBuffer(GL_ELEMENT_ARRAY_BUFFER),
//    mRenderMode(1),
    coff(0.3),
    current(0),
    prev(0),
    mNumIndices(2),
    diff(0.1),
    mMeshes(mNumIndices)
{
    mModel = math::Matrix4(1.0f);

    mMeshes[0].pos = { 0.0f, 0.0f, 0.0f };
    mMeshes[0].angle = NEWTON_BALL_DEFAULT_SPRING_ANG;
    mMeshes[0].weight = 1.0f;
    mMeshes[0].collide = false;

    mMeshes[1].pos = { 0.5f, -1.0f, 0.0f };
    mMeshes[1].angle = 0.0f;
    mMeshes[1].weight = 1.0f;
    mMeshes[1].collide = false;

    // mMeshes[2].pos = { 1.0f, -1.0f, 0.0f };
    // mMeshes[2].angle = 0.0f;
    // mMeshes[2].weight = 1.0f;
    // mMeshes[2].collide = false;

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

NewtonBall::~NewtonBall()
{
    // do destruction here
}

void NewtonBall::renderGeometry(math::Matrix4 const& projection,
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

void NewtonBall::updateGeometry(atlas::core::Time<> const& t)
{
    std::vector<mass> meshes = mMeshes;
    // for (int i = 0; i < mNumIndices; i++)
    // {

        // do something with meshes[i]
    // }

    // assume initial angle is 60 degress
    // float currentAng = meshes[0].angle;
    // float currentAngInRad = glm::radians(meshes[0].angle);
    // float c = std::cos(currentAngInRad)-0.5;
    // float inside = 2*fg[1]*c;
    // inside = 0.0;

    // math::Vector a = math::Vector(0, std::sqrt((math::Vector(0, 2, 0)*fg*math::Vector(0, (std::cos(glm::radians(meshes[0].angle))-0.5), 0))[1]), 0)/meshes[0].weight;
    // math::Vector a = math::Vector(0, inside, 0);
    // math::Vector a = fg/slt*std::sin(glm::radians(meshes[0].angle));
    // std::printf("angle: %f\n", (math::Vector(0, 2, 0)*fg*math::Vector(0, (std::cos(glm::radians(meshes[0].angle))-0.5), 0))[1]);
    // std::printf("Theta: %f\n", std::cos(glm::radians(NEWTON_BALL_DEFAULT_SPRING_ANG)));
    // math::Vector v = meshes[0].vt * t.deltaTime + 0.5f * a * t.deltaTime * t.deltaTime;

    // Update mass velocity
    // meshes[0].vt -= a * t.deltaTime;

    // math::Vector k = a * t.deltaTime;current
    // std::printf("v[1]: %f\n", v[1]);
    // std::printf("v[2]: %f\n", v[2]);
    // meshes[0].pos = meshes[0].pos + v;

    // Update mass angle
    meshes[current].angle += t.deltaTime * meshes[current].rate;
    meshes[current].rate -= t.deltaTime * (((fg/slt) * std::sin(meshes[current].angle))[1] + (coff/(meshes[current].weight*slt*slt)) * meshes[current].rate);
    // meshes[0].rate -= t.deltaTime * ((fg/slt) * std::sin(meshes[0].angle) + (0.1/(meshes[0].weight*slt*slt)) * meshes[0].rate);
    // std::printf("%f\n", ((fg/slt) * std::sin(meshes[0].angle) + (0.1/(meshes[0].weight*slt*slt)))[1]);
    // meshes[0].rate -= ((fg/slt*std::sin(glm::radians(meshes[0].angle)+0.1))[1] * meshes[0].rate) * t.deltaTime;
    // meshes[0].rate -= t.deltaTime * (fg/);
    // std::printf("%f\n", (fg/slt*std::sin(glm::radians(meshes[0].angle)+0.1)*meshes[0].rate) * t.deltaTime);
    // meshes[0].angle += v[1];
    // meshes[0].angle += 0.01;
    // std::printf("angle:%f\n", meshes[0].angle);
    // std::printf("rate:%f\n", meshes[0].rate);

    // meshes[0].angle = 20.0*std::cos(std::sqrt(glm::abs(fg)[1])*t.deltaTime);
    // std::printf("t.deltaTime: %f\n", t.deltaTime);

    // the distance from the origin
    // float sideAngle = (180-meshes[0].angle)/2.0f;
    // float d = slt*std::sin(glm::radians(meshes[0].angle))/std::sin(glm::radians(sideAngle));
    // meshes[0].pos = { d*std::cos(90-sideAngle), d*std::sin(90-sideAngle), 0.0f };
    // std::printf("x:%f\n", std::sin(meshes[0].angle));
    // std::printf("x:%f\n", std::sin(meshes[0].angle)*slt);
    // std::printf("y:%f\n", std::cos(meshes[0].angle));
    // std::printf("y:%f\n", std::cos(meshes[0].angle)*slt);
    meshes[current].pos = { std::sin(meshes[current].angle), std::cos(meshes[current].angle), 0.0f };

    if (current == 1) {
        meshes[current].pos[0] += 1;
    }

    // print KE and PE
    // std::printf("KE: %f\n", 0.5 * meshes[0].weight * slt * std::cos(meshes[0].angle) + 0.5 * meshes[0].weight * slt * std::sin(meshes[0].angle));
    // std::printf("PE: %f\n\n", meshes[0].weight * fg[1] * std::cos(meshes[0].angle));

    // std::printf("x:%f\n", glm::abs(0.5 * current - meshes[current].pos[0]));
    // std::printf("y:%f\n", glm::abs(meshes[current].pos[1] + 1));
    if (glm::abs(0.5 * current - meshes[current].pos[0]) < diff &&
        glm::abs(meshes[current].pos[1] + 1) < diff &&
        !meshes[current].collide) {
        // may not necessary work for multiple balls
        NewtonBall::setNextNewtonBall();
        meshes[current] = meshes[prev];
        meshes[prev].rate = 0;
        meshes[prev].angle = 0;
        meshes[prev].collide = true;
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

void NewtonBall::setNextNewtonBall()
{
    int temp;

    if (current == prev) {
        current++;
    } else if (current > prev && current + 1 < mNumIndices) {
        prev = current;
        current++;
    } else if (current > prev && current + 1 == mNumIndices || current < prev && current == 0) {
        temp = current;
        current = prev;
        prev = temp;
    } else if (current < prev && current > 0) {
        prev = current;
        current--;
    }
}