#include "Render/Entities/ShadowMapBakedRender.h"

using namespace CSEditor::Render;

ShadowMapBakedRender::ShadowMapBakedRender(ShadowMapBakedGeometry* _generator)
{
    setUseVertexBufferObjects(true);

    osg::Vec4Array* vAry2 = new osg::Vec4Array;
    vAry2->resize(_generator->getNumPrimitivesGenerated());
    setVertexArray(vAry2);
    addPrimitiveSet(new osg::DrawArrays(GL_LINES, 0, _generator->getNumPrimitivesGenerated()));

    osg::StateSet* sset = getOrCreateStateSet();
    ///hacking rendering order
    /*osg::BlendFunc* bf = new
    osg::BlendFunc(osg::BlendFunc::SRC_ALPHA,
    osg::BlendFunc::ONE_MINUS_SRC_ALPHA );
    sset->setAttributeAndModes(bf);*/

    sset->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
    getOrCreateVertexBufferObject();
    sset->setAttribute(createRenderShader());
}

osg::Program* ShadowMapBakedRender::createRenderShader()
{
    osg::Program* pgm = new osg::Program;
    pgm->setName("osg transformfeedback renderer demo");
    // TODO
    // pgm->addShader( new osg::Shader( osg::Shader::VERTEX,   RendervertSource ) );
    // pgm->addShader( new osg::Shader( osg::Shader::FRAGMENT, fragSource ) );
    return pgm;
}