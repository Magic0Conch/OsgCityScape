#pragma once

#include <string>
#include <vector>

namespace CSEditor::Resources{
    class Component;
    // CLASS(ComponentDefinitionRes, Fields)
    // {
    //     REFLECTION_BODY(ComponentDefinitionRes);

    // public:
    //     std::string m_type_name;
    //     std::string m_component;
    // };

    // REFLECTION_TYPE(ObjectDefinitionRes)
    // CLASS(ObjectDefinitionRes, Fields)
    // {
    //     REFLECTION_BODY(ObjectDefinitionRes);

    // public:
    //     std::vector<Reflection::ReflectionPtr<Component>> m_components;
    // };

    // REFLECTION_TYPE(ObjectInstanceRes)
    class ObjectInstanceRes{
    public:
        std::string              m_name;
        std::string              m_definition;

        // std::vector<Reflection::ReflectionPtr<Component>> m_instanced_components;
        std::vector<Component> m_instanced_components;
    };
}
