#ifndef OBJECT_LOADER_INCLUDE_MODEL_VIEWER_HPP
#define OBJECT_LOADER_INCLUDE_MODEL_VIEWER_HPP

#pragma once

#include "Model.hpp"

#include <atlas/tools/ModellingScene.hpp>

class ModelViewer : public atlas::tools::ModellingScene
{
public:
    ModelViewer();

    void renderScene() override;

private:
    int mCurrentModel;
    std::vector<Model> mModels;
};


#endif