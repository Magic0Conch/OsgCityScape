#include "Core/ECS/Components/ComponentFactory.h"
#include "Core/ECS/Components/Mesh.h"

using namespace  CSEditor::ECS;

std::shared_ptr<Component> ComponentFactory::createComponent(const std::string& type) {
    static const std::unordered_map<std::string, std::shared_ptr<Component>(*)()> creators = {
        {"Transform", []() -> std::shared_ptr<Component> { return std::make_shared<Transform>(); }},
        {"Mesh", []() -> std::shared_ptr<Component> { return std::make_shared<Mesh>(); }}
    };

    const auto it = creators.find(type);
    if (it != creators.end()) {
        return it->second();
    }

    return nullptr;
}