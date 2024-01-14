#include "Core/ECS/Components/Mesh.h"
#include "Core/ECS/Object.h"
#include "Editor/Core/RuntimeContext.h"
#include "Core/ECS/Components/Mesh.h"
#include "osgDB/ReadFile"
#include "Resources/ResourceManagement/ConfigManager.h"
#include <memory>
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

void Mesh::loadResource(std::shared_ptr<Object> parentObject){
    m_parentObject = parentObject;
    auto transformNode = parentObject->getTransformComponent().getNode();
    transformNode->addChild(meshNode);
    // osg::ref_ptr<osg::Geode> geodeNode = new osg::Geode;
    // geodeNode->addDrawable(meshNode);

}