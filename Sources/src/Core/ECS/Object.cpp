#include "Core/ECS/Object.h"

using namespace CSEditor::ECS;

Object::Object(ObjectID id) : m_id {id} {}
Object::~Object(){

}

void Object::tick(float delta_time){

}

bool Object::load(const ResourceType::ObjectInstance& object_instance_res){

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

}

std::vector<Component> Object::getComponents() { 
    return m_components; 
}

