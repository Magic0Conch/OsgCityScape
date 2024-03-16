#pragma once
#include "Core/ECS/Components/Material.h"
#include <unordered_map>
#include <string>
#include <memory>
namespace CSEditor::Resources
{
    class MaterialManager {
    public:
        static MaterialManager& getInstance();
        std::shared_ptr<ECS::Material> getMaterial(const std::string& path);

    private:
        MaterialManager() {}
        std::unordered_map<std::string, std::shared_ptr<ECS::Material>> materials;
    };
}