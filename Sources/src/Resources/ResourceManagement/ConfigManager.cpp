#include "Resources/ResourceManagement/ConfigManager.h"
#include <filesystem>
#include <fstream>
#include <string>
using namespace CSEditor::Resources;


ConfigManager::ConfigManager(const std::filesystem::path& configFilePath){
    initialize(configFilePath);
}

void ConfigManager::initialize(const std::filesystem::path& configFilePath){
    std::ifstream configFile(configFilePath);
    std::string configLine;
    while (std::getline(configFile, configLine))
    {
        size_t seperatePos = configLine.find_first_of('=');
        if (seperatePos > 0 && seperatePos < (configLine.length() - 1))
        {
            std::string name  = configLine.substr(0, seperatePos);
            std::string value = configLine.substr(seperatePos + 1, configLine.length() - seperatePos - 1);
            if (name == "RootFolder"){
                m_rootFolder = configFilePath.parent_path().parent_path() / value;
            }            
            else if (name == "BinaryRootFolder"){
                m_binaryFolder = configFilePath.parent_path().parent_path().parent_path()/value;
            }
            else if (name == "AssetFolder")
            {
                m_assetFolder = m_rootFolder / value;
            }
            else if (name == "DefaultWorld")
            {
                m_defaultWorldUrl = m_rootFolder / value;
            }
            else if (name == "ShaderFolder") {
                m_shaderRootPath = m_rootFolder / value;
            }
            else if (name == "MaterialFolder") {
                m_materialRootPath = m_rootFolder / value;
            }
            else if (name == "CaptureFolder") {
                m_captureImageFolder = m_rootFolder / value;
            }
        }
    }
}

const std::filesystem::path& ConfigManager::getRootFolder() const{
    return m_rootFolder;
}
const std::filesystem::path& ConfigManager::getAssetFolder() const{
    return m_assetFolder;
}

const std::filesystem::path& ConfigManager::getDefaultWorldUrl() const{
    return m_defaultWorldUrl;
}

const std::filesystem::path& ConfigManager::getShaderFolder() const{
    return m_shaderRootPath;
}

const std::filesystem::path& ConfigManager::getMaterialFolder() const{
    return m_materialRootPath;
}

const std::filesystem::path& ConfigManager::getBinaryFolder() const{
    return m_binaryFolder;
}

const std::filesystem::path& ConfigManager::getCaptureImageFolder() const{
    return m_captureImageFolder;
}