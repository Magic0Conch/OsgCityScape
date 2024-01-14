#pragma once
#include "Transform.h"
#include "Mesh.h"

namespace CSEditor::ECS {
    class ComponentFactory {
    public:
        static std::shared_ptr<Component> createComponent(const std::string& type);

        template<class T>
        static std::shared_ptr<T> createComponent() {
            std::string tmpname=typeid(T).name();
            auto n = tmpname.rfind("::");
            auto componentType = tmpname.substr(n+2);
            static const std::unordered_map<std::string, std::shared_ptr<Component>(*)()> creators = {
                {"Transform", []() -> std::shared_ptr<Component> { return std::make_shared<Transform>(); }},
                {"Mesh", []() -> std::shared_ptr<Component> { return std::make_shared<Mesh>(); }}
            };

            const auto it = creators.find(componentType);
            std::shared_ptr<T> component;
            if (it != creators.end()) {
                component = it->second();
            }
            component->setTypeName(componentType);
            return component;
        }
    private:
        ComponentFactory() = default;
    };
}

