#include "Core/Helpers/Serializer.h"
#include "Core/Helpers/ISerializer.h"

using namespace CSEditor::Helpers;

void Serializer::serializeVector3f(Json& json,const osg::Vec3f& value){

}
void Serializer::serializeVector4f(Json& json,const osg::Vec4f& value){

}

void Serializer::deserializeVector3f(const Json& json,osg::Vec3f& value){
    auto x = json["x"].number_value();
    auto y = json["y"].number_value();
    auto z = json["z"].number_value();
    value = osg::Vec3f(x,y,z);
}

void Serializer::deserializeVector4f(const Json& json,osg::Vec4f& value){
    auto x = json["x"].number_value();
    auto y = json["y"].number_value();
    auto z = json["z"].number_value();
    auto w = json["w"].number_value();
    value = osg::Vec4f(x,y,z,w);
}
