#pragma once
#include <memory>
#include <vector>
#include <string>
#include "Core/Helpers/ISerializer.h"
#include "Core/ECS/Components/Component.h"
namespace CSEditor::ResourceType {

class ObjectInstance: public Helpers::ISerializable
{
public:
    virtual void serialize(Json& jsonObject) override;
    virtual void deserialize(Json& jsonObject) override;
    
    std::string getName() const;
    std::string getDefinitionUrl() const;
    std::vector<std::shared_ptr<ECS::Component>>getInstancedComponents() const;
    void addInstanceComponent(std::shared_ptr<ECS::Component> component);
    void setName(const std::string& name);
    void setDefinitionUrl(const std::string& definitionUrl);
    void setInstancedComponents(const std::vector<std::shared_ptr<ECS::Component>>& instancedComponents);
private:
    std::string m_name;
    std::string m_definitionUrl;
    std::vector<std::shared_ptr<ECS::Component>> m_instancedComponents;
};
}