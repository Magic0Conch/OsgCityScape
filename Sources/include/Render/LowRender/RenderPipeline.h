#ifndef RENDER_PIPELINE_H
#define RENDER_PIPELINE_H
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <variant>
#include <vector>
#include <osg/ref_ptr>
#include "RTTCamera.h"
#include "osg/Node"
#include "osg/Object"
#include "osg/Referenced"
#include "osg/Vec2"
#include "osg/Vec3"
#include "osg/Vec4"

namespace CSEditor::Render{
class RenderPipeline:public osg::Referenced{
public:
    virtual void addRenderPass(const osg::ref_ptr<Material> pass);
    virtual void addRenderPipeline(const osg::ref_ptr<RenderPipeline> rhs);
    std::vector<osg::ref_ptr<Material>> getRenderPasses() const;
protected:
    std::vector<osg::ref_ptr<Material>> m_renderPasses;
};

class RenderPipelinePostProcess:public RenderPipeline{
public:
    void addRenderPassesToOsgGroup(osg::Group& group) const;
    osg::ref_ptr<osg::Geode> getDestinationQuadGeode() const;
    void enableUpdateUniformPerFrame();
};

class UpdateUniformCallback:public osg::NodeCallback{
public:
    UpdateUniformCallback(RenderPipelinePostProcess* rp);
    UpdateUniformCallback(std::shared_ptr<RenderPipelinePostProcess> rp);
    UpdateUniformCallback(Material* rp);
    UpdateUniformCallback(std::shared_ptr<Material> rp);
    virtual void operator()(osg::Node* node,osg::NodeVisitor* nv);
protected:
    virtual void setUniforms();
private:
    using RpType = std::variant<Material,RenderPipelinePostProcess>;
    void updateAttributeList();
    void setAttribute(const std::string& pname,const Material::Attribute pval,Material& pass);
private:
    std::shared_ptr<RpType> m_rp;
};

}

#endif