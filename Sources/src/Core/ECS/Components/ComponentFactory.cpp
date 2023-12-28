#include "Core/ECS/Components/ComponentFactory.h"

using namespace  CSEditor::ECS;

std::shared_ptr<Component> ComponentFactory::createComponent(const std::string& type) {
    static const std::unordered_map<std::string, std::shared_ptr<Component>(*)()> creators = {
        {"Transform", []() -> std::shared_ptr<Component> { return std::make_shared<Transform>(); }}
    };

    const auto it = creators.find(type);
    if (it != creators.end()) {
        return it->second();
    }

    return nullptr;
}