#pragma once
#include "Core/ECS/Level.h"
#include "Render/Config/RenderConfig.h"
#include "Render/Entities/ArrowGizmo.h"
#include "osg/Matrixd"
#include <memory>
#include "Render/Effects/OutlineFX.h"
namespace CSEditor::Render {
class ObjectPickerPass{
public:
    ObjectPickerPass(std::shared_ptr<osg::Matrixd> projectionMatrix);

    void highlightSelectedObject(const RenderPipelineState renderPipelineState);

private:
    std::shared_ptr<ECS::Level> m_level;
    std::shared_ptr<osg::Matrixd> m_projectionMatrix;
    ECS::ObjectID m_lastSelectedObjectID = -1;
    osg::ref_ptr<osgFX::OutlineFX> m_outlinePass;
    osg::ref_ptr<osg::Group> m_selectedGroupNode;
};
}
