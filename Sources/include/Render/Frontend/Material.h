#ifndef MATERIAL_H
#define MATERIAL_H
#include <iostream>
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
#include "osg/Shader"
#include "osg/Vec2"
#include "osg/Vec3"
#include "osg/Vec4"
#include "Resources/Loaders/ShaderUtils.h"
namespace CSEditor::Resources{
class Material:public osg::Node{
public:
    using Attribute = std::variant<int*,bool*,float*,double*,osg::Vec2*,osg::Vec3*,osg::Vec4*>;
protected:
    osg::ref_ptr<osg::Geode> m_sourceGeode = nullptr;
    std::vector<std::pair<std::shared_ptr<std::string>, std::shared_ptr<Attribute>>> m_attributeList;
    
    std::string m_vertShaderPath;
    std::string m_fragShaderPath;
public:
    Material(osg::Geode* source):m_sourceGeode(source){}
    Material(osg::Geode* source,const std::string& vertPath,const std::string& fragPath)
    :m_sourceGeode(source),m_vertShaderPath(vertPath),m_fragShaderPath(fragPath){
        ShaderUtils::setShaderProgram(m_sourceGeode->getOrCreateStateSet(),m_vertShaderPath, m_fragShaderPath);
    }
    Material(const std::string& vertPath,const std::string& fragPath)
    :m_vertShaderPath(vertPath),m_fragShaderPath(fragPath){}
    Material() = default;
    void addUniform(osg::Uniform* uniform){
        m_sourceGeode->getOrCreateStateSet()->addUniform(uniform);
    }
    
    template<class T>
    void addUniform(const std::string& name,T* val,bool updatePerFrame = true){
        m_sourceGeode->getOrCreateStateSet()->addUniform(new osg::Uniform(name.c_str(),*val));
        std::shared_ptr<std::string> pname = std::make_shared<std::string>(name);
        std::shared_ptr<Attribute> pval = std::make_shared<Attribute>(val);
        if(updatePerFrame)
            m_attributeList.emplace_back(std::make_pair(pname, pval));
    }
    
    template<class T>
    void addUniform(const std::string& name,const T& val){
        m_sourceGeode->getOrCreateStateSet()->addUniform(new osg::Uniform(name.c_str(),val));
    }

    template<class T>
    void setUniform(const std::string& name,const T& val){
        auto ss = m_sourceGeode->getOrCreateStateSet();
        ss->getUniform(name)->set(val);
    }

    std::vector<std::pair<std::shared_ptr<std::string>, std::shared_ptr<Attribute>>>getAttributeList(){
        return m_attributeList;
    }

};
}

#endif