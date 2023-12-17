#include "Resources/ResourceManagement/AssetManager.h"
#include "Resources/ResourceManagement/ConfigManager.h"
#include "Editor/Core/RuntimeContext.h"
#include <filesystem>

using namespace CSEditor::Resources;

std::filesystem::path AssetManager::getFullPath(const std::string &relativePath) const{
    return std::filesystem::absolute(Core::g_runtimeContext.configManager->getRootFolder() / relativePath);
}