#include "Render/Entities/ShadowMapBakedGeometry.h"
#include "Editor/Core/RuntimeContext.h"
#include "GL/glcorearb.h"
#include "Resources/ResourceManagement/ConfigManager.h"
#include "Resources/Loaders/ShaderLoader.h"

using namespace CSEditor::Render;

ShadowMapBakedGeometry::ShadowMapBakedGeometry():osg::Geometry(){
    init();
}

void ShadowMapBakedGeometry::init(){
    setUseVertexBufferObjects(true);

    osg::StateSet* sset = getOrCreateStateSet();
    sset->setMode( GL_LIGHTING, osg::StateAttribute::OFF );
    createTextureProjectionShader(sset);
}

void ShadowMapBakedGeometry::setRenderer(osg::Geometry*renderer){
    genbuffer = renderer->getOrCreateVertexBufferObject();
}

GLuint ShadowMapBakedGeometry::getNumPrimitivesGenerated() const {
    return _vertexArray->getElementSize();
    return 0;
}

void ShadowMapBakedGeometry::drawImplementation( osg::RenderInfo& renderInfo ) const{
    unsigned int contextID = renderInfo.getState()->getContextID();

    GLuint ubuff= genbuffer->getOrCreateGLBufferObject(contextID)->getGLObjectID();

    osg::GLExtensions* ext = renderInfo.getState()->get<osg::GLExtensions>();

    ext->glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0,ubuff);

    glEnable(GL_RASTERIZER_DISCARD);

    ext->glBeginTransformFeedback(GL_TRIANGLES);


    osg::Geometry::drawImplementation(  renderInfo );


    ext->glEndTransformFeedback();

    glDisable(GL_RASTERIZER_DISCARD);

    ext->glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, 0);
}

void ShadowMapBakedGeometry::createTextureProjectionShader(osg::StateSet* ss){
    const std::string& vertPath = (Core::g_runtimeContext.configManager->getShaderFolder() / "common/ShadowMapBakedGeometry.vert").string();
    osg::ref_ptr<osg::Program> program = Resources::ShaderLoader::create(vertPath);   
    program->addTransformFeedBackVarying(std::string("out1"));
    program->setTransformFeedBackMode(GL_INTERLEAVED_ATTRIBS);

}