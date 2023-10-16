#pragma once
#include <memory>
#include <string>
#include <utility>
#include <variant>
#include <vector>
#include <windows.h>
#include <osg/Camera>
#include <osg/Geode>
#include <osg/Group>
#include <osg/Referenced>
#include <osg/Uniform>
#include <osg/Vec4f>
#include <osg/ref_ptr>
#include "osg/Geometry"
#include "osg/Node"
#include "osg/Object"
#include "osg/Program"
#include "osg/Shader"
#include "osg/StateSet"
#include "osg/Vec2"
#include "osg/Vec3"
#include "osg/Vec4"
#include "Resources/Loaders/ShaderLoader.h"
namespace CSEditor::Resources{
class Material:public osg::Referenced{
public:
    using Attribute = std::variant<int*,bool*,float*,double*,osg::Vec2*,osg::Vec3*,osg::Vec4*>;
protected:
    std::vector<std::pair<std::shared_ptr<std::string>, std::shared_ptr<Attribute>>> m_attributeList;
    osg::ref_ptr<osg::Program> m_shaderProgram;
    osg::ref_ptr<osg::StateSet> m_stateSet;
public:
    Material() = default;
    Material(const std::string& vertPath,const std::string& fragPath);
    Material(const osg::ref_ptr<osg::Program> shaderProgram);

    void addUniform(osg::Uniform* uniform);
    
    std::vector<std::pair<std::shared_ptr<std::string>, std::shared_ptr<Attribute>>>getAttributeList() const;
    osg::ref_ptr<osg::StateSet> getStateSet() const;


    template<class T>
    void addUniform(const std::string& name,T* val,bool updatePerFrame = true);
    
    template<class T>
    void addUniform(const std::string& name,const T& val);

    template<class T>
    void setUniform(const std::string& name,const T& val);

    void bind(osg::ref_ptr<osg::Geode> geode);
};
}

#include "Material.inl"