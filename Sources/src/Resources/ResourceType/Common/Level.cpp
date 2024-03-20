#include "Resources/ResourceType/Common/Level.h"
#include "Resources/ResourceType/Common/Object.h"
#include "osg/Vec3f"

using namespace CSEditor::ResourceType;

void Level::serialize(Json& jsonBody){
    Json::object root = Json::object{}; 
    
    // root["level_urls"] = getLevelUrls();
    // root["name"] = getName();
    // root["default_level_url"] = getDefaultLevelUrl();        
}

void Level::deserialize(Json& jsonBody){
    auto gravityJson = jsonBody["gravity"];
    setGravity(gravityJson["x"].number_value(),gravityJson["y"].number_value(),gravityJson["z"].number_value());
    auto objectsJson = jsonBody["objects"];
    auto objectsJsonArray = objectsJson.array_items();
    for(auto& objectJson:objectsJsonArray){
        ObjectInstance objectInstance;
        objectInstance.deserialize(objectJson);
        addObject(objectInstance);
    }
    
}

osg::Vec3f Level::getGravity() const {
    return m_gravity;
}

void Level::setGravity(const osg::Vec3f& gravity) {
    m_gravity = gravity;
}

void Level::setGravity(const float x,const float y,const float z) {
    m_gravity = osg::Vec3f(x,y,z);
}

std::vector<ObjectInstance> Level::getObjects() const {
    return m_objects;
}

void Level::setObjects(const std::vector<ObjectInstance>& objects) {
    m_objects = objects;
}

void Level::addObject(const ObjectInstance &object){
    m_objects.emplace_back(object);
}

void Level::loadResource(){
        
}