#pragma once

#include "Material.h"

namespace CSEditor::Resources{
template<class T>
void Material::addUniform(const std::string& name,T* val,bool updatePerFrame){
    m_sourceGeode->getOrCreateStateSet()->addUniform(new osg::Uniform(name.c_str(),*val));
    std::shared_ptr<std::string> pname = std::make_shared<std::string>(name);
    std::shared_ptr<Attribute> pval = std::make_shared<Attribute>(val);
    if(updatePerFrame)
        m_attributeList.emplace_back(std::make_pair(pname, pval));
}

template<class T>
void Material::addUniform(const std::string& name,const T& val){
    m_sourceGeode->getOrCreateStateSet()->addUniform(new osg::Uniform(name.c_str(),val));
}

template<class T>
void Material::setUniform(const std::string& name,const T& val){
    auto ss = m_sourceGeode->getOrCreateStateSet();
    ss->getUniform(name)->set(val);
}
}

