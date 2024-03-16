#pragma once

#include <filesystem>

namespace CSEditor::Resources
{
    class ConfigManager
    {
    public:
        ConfigManager(const std::filesystem::path& config_file_path);
        void initialize(const std::filesystem::path& config_file_path);

        const std::filesystem::path& getRootFolder() const;
        const std::filesystem::path& getAssetFolder() const;
        // const std::filesystem::path& getSchemaFolder() const;
        // const std::filesystem::path& getEditorBigIconPath() const;
        // const std::filesystem::path& getEditorSmallIconPath() const;
        // const std::filesystem::path& getEditorFontPath() const;

        const std::filesystem::path& getDefaultWorldUrl() const;
        const std::filesystem::path& getShaderFolder() const;
        const std::filesystem::path& getMaterialFolder() const;
        // const std::string& getGlobalRenderingResUrl() const;

    private:
        std::filesystem::path m_rootFolder;
        std::filesystem::path m_assetFolder;
        std::filesystem::path m_defaultWorldUrl;
        std::filesystem::path m_shaderRootPath;
        std::filesystem::path m_materialRootPath;
    };
} 
