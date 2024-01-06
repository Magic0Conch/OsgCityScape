#include "Core/ECS/Components/Mesh.h"
#include "Editor/Core/RuntimeContext.h"
#include "osgDB/ReadFile"
#include "Resources/ResourceManagement/ConfigManager.h"
#include <string>

using namespace CSEditor::ECS;

void Mesh::serialize(Json &jsonObject){

}

void Mesh::deserialize(Json &jsonObject){
    const auto modelPath = jsonObject["modelPath"].string_value();
    const auto absoluteModelPath = Core::g_runtimeContext.configManager->getAssetFolder()/modelPath;
    const auto absoluteModelPathString = absoluteModelPath.string();
    meshNode = osgDB::readNodeFile(absoluteModelPathString);
}

