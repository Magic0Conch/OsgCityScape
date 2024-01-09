#include "Core/ECS/Object.h"
#include "Editor/Core/RuntimeContext.h"
#include <utility>

using namespace CSEditor::ECS;

Object::Object(ObjectID id,Transform& transform,ObjectID parentId) : m_id {id} ,m_transform(transform),m_parentId(parentId){}
Object::~Object(){

}

void Object::tick(float delta_time){

}

bool Object::load(const ResourceType::ObjectInstance& objectInstanceRes){
    m_components.clear();
    setName(objectInstanceRes.getName());
    auto componentInstances = objectInstanceRes.getInstancedComponents();
    for(auto component:componentInstances){
        m_components.emplace_back(std::make_pair(component->returnTypeName(), component));
    }
    for (auto component : m_components){
        if (component.second){
            component.second->loadResource(shared_from_this());
        }
    }
    setDefinitionUrl(objectInstanceRes.getDefinitionUrl());
    return true;
}

void Object::save(ResourceType::ObjectInstance& out_object_instance_res){

}

ObjectID Object::getID() const {
    return m_id; 
}

void Object::setName(std::string name) { 
    m_name = name; 
}

const std::string& Object::getName() const {
    return m_name; 
}

bool Object::hasComponent(const std::string& compenent_type_name) const{
    return true;
}

std::vector<std::pair<std::string,std::shared_ptr<Component>>> Object::getComponents() { 
    return m_components; 
}

void Object::setDefinitionUrl(const std::string& definitionUrl){
    m_definitionUrl = definitionUrl;
}

Transform& Object::getTransformComponent(){
    return m_transform;
}

const ObjectID Object::getParentId(){
    return m_parentId;
}