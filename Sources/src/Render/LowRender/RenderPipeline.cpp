#include "Render/LowRender/RenderPipeline.h"
#include "Render/LowRender/RTTCamera.h"
#include <memory>
#include <variant>

using namespace CSEditor::Render;

// void RenderPipeline::addRenderPass(const osg::ref_ptr<Material> pass){
//     m_renderPasses.emplace_back(pass);
// }

void RenderPipeline::addRenderPass(const osg::ref_ptr<RTTCamera> pass){
    m_renderPasses.emplace_back(pass);
}

void RenderPipeline::addRenderPipeline(const osg::ref_ptr<RenderPipeline> rhs){
    for (auto& pass : rhs->getRenderPasses()) {
        m_renderPasses.emplace_back(pass);
    }
}
std::vector<osg::ref_ptr<RTTCamera>> RenderPipeline::getRenderPasses() const{
    return m_renderPasses;
}
void RenderPipelinePostProcess::addRenderPassesToOsgGroup(osg::Group& group) const{
    for (const auto& pass : m_renderPasses) {
        auto rttCamera = dynamic_cast<RTTCamera*>(pass.get())->getRTTCamera();
        group.addChild(rttCamera);
    }
}

void RenderPipelinePostProcess::enableUpdateUniformPerFrame(){
    getDestinationQuadGeode()->addUpdateCallback(new UpdateUniformCallback(this));
}


osg::ref_ptr<osg::Geode> RenderPipelinePostProcess::getDestinationQuadGeode() const{
    auto rttCamera = dynamic_cast<RTTCamera*>(m_renderPasses.back().get());
    return rttCamera->getDestinationQuadGeode();
}

void UpdateUniformCallback::setAttribute(const std::string& pname,const Resources::Material::Attribute pval,Material& pass){
    if(std::holds_alternative<int*>(pval)){
        pass.setUniform(pname, *std::get<int*>(pval));
    }
    else if(std::holds_alternative<bool*>(pval)){
        pass.setUniform(pname, *std::get<bool*>(pval));
    }
    else if(std::holds_alternative<float*>(pval)){
        pass.setUniform(pname, *std::get<float*>(pval));
    }
    else if(std::holds_alternative<double*>(pval)){
        pass.setUniform(pname, *std::get<double*>(pval));
    }
    else if(std::holds_alternative<osg::Vec2*>(pval)){
        pass.setUniform(pname, *std::get<osg::Vec2*>(pval));
    }
    else if(std::holds_alternative<osg::Vec3*>(pval)){
        pass.setUniform(pname, *std::get<osg::Vec3*>(pval));
    }
    else if(std::holds_alternative<osg::Vec4*>(pval)){
        pass.setUniform(pname, *std::get<osg::Vec4*>(pval));
    }
    else{
        std::cout<<"Type doesn't exist!"<<std::endl;
    }
}

void UpdateUniformCallback::updateAttributeList(){
    if(std::holds_alternative<RenderPipelinePostProcess>(*m_rp)){
        auto rp = std::get<RenderPipelinePostProcess>(*m_rp);
        for(const auto& pass:rp.getRenderPasses()){
            auto material = pass->getMaterial();
            if(material==nullptr)
                continue;
            for(const auto& attributePair:material->getAttributeList()){
                const auto& pname = *attributePair.first;
                const auto& pval = *attributePair.second;
                // int,bool,float,double,osg::Vec2,osg::Vec3,osg::Vec4
                setAttribute(pname, pval, *pass->getMaterial());
            }
        }
    }
    else if(std::holds_alternative<Material>(*m_rp)){
        auto pass = std::get<Material>(*m_rp);
        for(const auto& attributePair:pass.getAttributeList()){
            const auto& pname = *attributePair.first;
            const auto& pval = *attributePair.second;
            setAttribute(pname, pval, pass);
        }
    }
}

void UpdateUniformCallback::setUniforms(){}

UpdateUniformCallback::UpdateUniformCallback(Material* rp){
    // m_rp = std::make_shared<RpType>(rp);
    RpType rp_ = *rp;
    m_rp = std::make_shared<RpType>(rp_);
};
UpdateUniformCallback::UpdateUniformCallback(std::shared_ptr<Material> rp){
    RpType rp_ = *rp;
    m_rp = std::make_shared<RpType>(rp_);
};


UpdateUniformCallback::UpdateUniformCallback(RenderPipelinePostProcess* rp){
    RpType rp_ = *rp;
    m_rp = std::make_shared<RpType>(rp_);
};
UpdateUniformCallback::UpdateUniformCallback(std::shared_ptr<RenderPipelinePostProcess> rp){
    RpType rp_ = *rp;
    m_rp = std::make_shared<RpType>(rp_);
};

void UpdateUniformCallback::operator()(osg::Node* node,osg::NodeVisitor* nv){
    updateAttributeList();
    setUniforms();
    traverse(node, nv);
}