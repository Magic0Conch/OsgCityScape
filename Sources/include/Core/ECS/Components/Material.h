#pragma once
#include "Component.h"
#include "Core/ECS/Components/Component.h"
#include "Core/ECS/ObjectIDAllocator.h"
#include "osg/Geometry"
#include "osg/NodeVisitor"
#include "osg/Program"
#include "osg/StateSet"
#include "osg/Texture2D"
#include "osg/Vec2f"
#include "osg/Vec3f"
#include "osg/Vec4f"
#include "osg/ref_ptr"
#include <any>
#include <osg/PositionAttitudeTransform>
#include <string>
#include <unordered_map>
#include <vector>

namespace CSEditor::ECS {

class Material:public Component{
public:
    Material();
    Material(const std::string& vertPath,const std::string& fragPath);
    template<typename T>
    bool isType(const std::any& operand) {
        return operand.type() == typeid(T);
    }   
    virtual void serialize(Json& jsonObject) override;
    virtual void deserialize(Json& jsonObject) override;
    virtual void loadResource(std::shared_ptr<Object> parentObject) override;
    
    void bind(osg::ref_ptr<osg::Geode> geode);
    void bind(osg::ref_ptr<osg::Geometry> geom);

    template<class T>
    void updateUniform(const std::string& name,const T& value){
        auto uniform = m_stateSet->getUniform(name);
        if(uniform){
            uniform->set(value);
        }
        else{
            osg::ref_ptr<osg::Uniform> uniform = new osg::Uniform(name.c_str(), value);
            m_stateSet->addUniform(uniform.get());
        }
    }
        
    void setStateSet(osg::ref_ptr<osg::StateSet> stateSet);
    void setName(const std::string& name);
    void setVertPath(const std::string& vertPath);
    void setFragPath(const std::string& fragPath);

    // void setTextures(const std::unordered_map<std::string, osg::ref_ptr<osg::Texture2D>>& textures);
    // void setInts(const std::unordered_map<std::string, int>& ints);
    // void setFloats(const std::unordered_map<std::string, float>& floats);

    // For Vec2f, Vec3f and Vec4f setters we assume you want to add or update individual entries.
    void setTexture(const std::string& name, const osg::ref_ptr<osg::Texture2D>& texture);
    void setInt(const std::string& name, int value);
    void setFloat(const std::string& name, float value);
    void setVec2(const std::string& name, const osg::Vec2f& value);
    void setVec3(const std::string& name, const osg::Vec3f& value);
    void setVec4(const std::string& name, const osg::Vec4f& value);
    void setMatrix(const std::string& name, const osg::Matrixf& value);
    void setMatrix(const std::string& name, const osg::Matrixd& value);
    void setBool(const std::string& name, bool value);
    void addTexture(const std::string &key,osg::ref_ptr<osg::Texture2D> value);
	void addInt (const std::string &key,int value);
	void addFloat (const std:: string &key,float value);
	
	// Getters
	osg::ref_ptr<osg::StateSet> getStateSet() const;
	const std::string& getName() const;
	const std::string& getVertPath() const;
	const std::string& getFragPath() const;
	
	// const std::unordered_map<std::string, osg::ref_ptr<osg::Texture2D>>& getTextures ()const;
	// const std::unordered_map<std::string,int>& getInt ()const;
	// const std::unordered_map<std::string,float >& getFloat ()const;
    template<class T>
    T getProperty(const std::string& key){
        return std::any_cast<T>(m_attributes[key]);
    }
private:
    std::string m_name;
    std::string m_vertPath;
    std::string m_fragPath;
    osg::ref_ptr<osg::StateSet> m_stateSet;
    std::unordered_map<std::string, std::any> m_attributes;
    osg::ref_ptr<osg::Program> m_program;
};
}
