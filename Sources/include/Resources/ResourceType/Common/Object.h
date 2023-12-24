#pragma once
#include <vector>
#include <string>
namespace CSEditor::ResourceType {
class Component;

class ObjectInstance
{
public:

private:
    std::string m_name;
    std::string m_definitionUrl;
    std::vector<Component> m_instancedComponents;
};
}