#include "Resources/ResourceManagement/MaterialManager.h"
#include "Editor/Core/RuntimeContext.h"
#include "Resources/ResourceManagement/ConfigManager.h"

using namespace CSEditor::Resources;

MaterialManager& MaterialManager::getInstance() {
    static MaterialManager instance;
    return instance;
}

std::shared_ptr<CSEditor::ECS::Material> MaterialManager::getMaterial(const std::string& path) {
    auto it = materials.find(path);
    if (it != materials.end()) {
        return it->second;
    } 
    else {
        auto materialPath = Core::g_runtimeContext.configManager->getMaterialFolder()/path;
        std::shared_ptr<CSEditor::ECS::Material> material = std::make_shared<CSEditor::ECS::Material>();
        Core::g_runtimeContext.assetManager->deserialize(materialPath.string(),(*material));
        materials[path] = material;
        return material;
    }
}