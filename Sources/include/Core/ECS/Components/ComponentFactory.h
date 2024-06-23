#pragma once
#include "Core/ECS/Components/CompositeMesh/HighlightArea.h"
#include "Transform.h"
#include "ModelMesh.h"

namespace CSEditor::ECS {
    class ComponentFactory {
    public:
        template<typename... Args>
        static std::shared_ptr<Component> createComponent(const std::string& type,Args&&... args) {
            static const std::unordered_map<std::string, std::function<std::shared_ptr<Component>(Args&&...)>> creators = {
                {"Transform", [](Args&&... args) -> std::shared_ptr<Component> { return std::make_shared<Transform>(std::forward<Args>(args)...); }},
                {"ModelMesh", [](Args&&... args) -> std::shared_ptr<Component> { return std::make_shared<ModelMesh>(std::forward<Args>(args)...); }},
                {"HighlightArea", [](Args&&... args) -> std::shared_ptr<Component> { return std::make_shared<HighlightArea>(std::forward<Args>(args)...); }}
            };

            const auto it = creators.find(type);
            std::shared_ptr<Component> component;
            if (it != creators.end()) {
                std::shared_ptr<Component> component = it->second(std::forward<Args>(args)...);
                component->setTypeName(type);
                return component;
            }
            return nullptr;
            // std::shared_ptr<Component> component = std::make_shared<ComponentType>(std::forward<Args>(args)...);
        }

        template<typename T,typename... Args>
        static std::shared_ptr<T> createComponent(Args&&... args) {
            std::string tmpname=typeid(T).name();
            auto n = tmpname.rfind("::");
            auto componentType = tmpname.substr(n+2);
            std::shared_ptr<T> component = std::make_shared<T>(std::forward<Args>(args)...);
            component->setTypeName(componentType);
            return component;
        }
    private:
        ComponentFactory() = default;
    };
}

