#include "Core/ECS/Components/Component.h"
using namespace CSEditor::ECS;

void Component::serialize(Json& jsonObject){


}
void Component::deserialize(Json& jsonObject){
    const auto typeName = jsonObject["typeName"].string_value();
    if(typeName == "TransformComponent"){
        
    }

}
