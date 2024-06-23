#include "Core/ECS/Components/Component.h"
#include <memory>
#include <string>
using namespace CSEditor::ECS;

void Component::serialize(Json& jsonObject){


}
void Component::deserialize(Json& jsonObject){

}

void Component::loadResource(std::shared_ptr<Object> parentObject){
    m_parentObject = parentObject;
    onComponentAdded(parentObject);
}

void Component::onComponentAdded(std::shared_ptr<Object> parentObject){

}

std::string Component::returnTypeName(){
    return m_typename;
}

void Component::setTypeName(std::string typeName){
    m_typename = typeName;
}

void Component::drawImGui(){

}