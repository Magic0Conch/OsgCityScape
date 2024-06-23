#include "Core/ECS/Object.h"
#include "Core/ECS/Components/Transform.h"
#include "Editor/Core/RuntimeContext.h"
#include <memory>
#include <utility>

using namespace CSEditor::ECS;

Object::Object(ObjectID id,ObjectID parentId) : m_id {id} ,m_parentId(parentId){}
Object::Object() : m_id (-1) ,m_parentId(-1){
}

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
        if(component.first == "Transform"){
            auto transformComponent = std::dynamic_pointer_cast<Transform>(component.second);
            m_transform = transformComponent;
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

std::vector<std::pair<std::string,std::shared_ptr<Component>>>& Object::getComponents() { 
    return m_components; 
}

void Object::setDefinitionUrl(const std::string& definitionUrl){
    m_definitionUrl = definitionUrl;
}

Transform& Object::getTransformComponent(){
    return *m_transform;
}

const ObjectID Object::getParentId(){
    return m_parentId;
}

void Object::setTransform(Transform* transform){
    m_transform.reset(transform);
}

void Object::setTransform(std::shared_ptr<Transform> other){
    // if(m_transform == nullptr){
    //     // auto p = std::make_pair("Transform", other);
    //     // m_components.emplace_back(p);
    //     addComponent<Transform>();
    // }
    m_transform = other;    
}

void Object::addChild(Object& childObject){
    getTransformComponent().addChild(childObject.getTransformComponent());
}

void Object::addChild(Transform& childTransform){
    getTransformComponent().addChild(childTransform);
}

const bool Object::isLeaf() {
    return getTransformComponent().isLeaf();
}

const std::vector<int>& Object::getChildIndex() const{
    return m_transform->getChildIndex();
}

void Object::setIsSelected(bool isSelected){
    m_isSelected = isSelected;
    if(isSelected){
        
    }
}
const bool Object::getIsSelected() const{
    return m_isSelected;
}

void Object::setShowTransform(bool showTransform){
    m_showTransform = showTransform;

}

const bool Object::getShowTransform() const{
    return m_showTransform;
}
