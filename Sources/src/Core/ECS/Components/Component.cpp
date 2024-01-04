#include "Core/ECS/Components/Component.h"
using namespace CSEditor::ECS;

void Component::serialize(Json& jsonObject){


}
void Component::deserialize(Json& jsonObject){

}

void Component::postLoadResource(std::weak_ptr<Object> parentObject){
    m_parentObject = parentObject;
}