#pragma once
#include <string>
#include <json11.hpp>
#include "osg/Vec3f"
#include "osg/Vec4f"
using Json = json11::Json;
namespace CSEditor::Helpers
{
	class Serializer
	{
	public:
		// static void SerializeBoolean(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node, const std::string& p_name, bool p_value);
		static void serializeVector3f(Json& json,const osg::Vec3f value);
		static void serializeVector4f(Json& json,const osg::Vec4f value);

		static void deserializeVector3f(const Json& json,osg::Vec3f value);
		static void deserializeVector4f(const Json& json,osg::Vec4f value);
		
	};
}