#include "Resources/ResourceType/Common/Object.h"
#include "Core/ECS/Components/Component.h"

using namespace CSEditor::ResourceType;

void ObjectInstance::serialize(Json& jsonBody){
    Json::object root = Json::object{}; 
    // root["level_urls"] = getLevelUrls();
    // root["name"] = getName();
    // root["default_level_url"] = getDefaultLevelUrl();        
}

void ObjectInstance::deserialize(Json& jsonBody){
    const auto name = jsonBody["name"].string_value();
    const auto definition = jsonBody["definition"].string_value();
    setName(name);
    setDefinitionUrl(definition);
    auto componentsInstanceJson = jsonBody["instanced_components"].array_items();
    for (auto& componentJson:componentsInstanceJson) {
        ECS::Component componentInstance;
        componentInstance.deserialize(componentJson);
        addInstanceComponent(componentInstance);
    } 
}

std::string ObjectInstance::getName() const {
    return m_name;
}

std::string ObjectInstance::getDefinitionUrl() const {
    return m_definitionUrl;
}

std::vector<CSEditor::ECS::Component> ObjectInstance::getInstancedComponents() const {
    return m_instancedComponents;
}

void ObjectInstance::addInstanceComponent(ECS::Component& component){
    m_instancedComponents.emplace_back(component);
}

void ObjectInstance::setName(const std::string& name) {
    m_name = name;
}

void ObjectInstance::setDefinitionUrl(const std::string& definitionUrl) {
    m_definitionUrl = definitionUrl;
}

void ObjectInstance::setInstancedComponents(const std::vector<ECS::Component>& instancedComponents) {
    m_instancedComponents = instancedComponents;
}
