// EditorInputManager.h
#pragma once

#include "Core/Event.h"
#include "osg/Vec2d"
#include "osg/Vec2f"
#include "osg/ref_ptr"
#include "osgViewer/Viewer"
#include <memory>
#include <osgGA/GUIEventHandler>
#include <osg/Camera>
#include <osg/Vec3d>
#include <osg/Quat>

namespace CSEditor::Core {
class EditorInputManager : public osgGA::GUIEventHandler {
public:
    EditorInputManager(osg::Camera* camera);
    virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa) override;
    void setMouseSensitivity(double sensitivity);
    void setMovementSpeed(double speed);
    void moveCamera(const osg::Vec3d& delta);
    void setScenePosition(const osg::Vec2f& position);
    osg::Vec2f getScenePosition() const;
private:
    osg::ref_ptr<osg::Camera> m_camera;
    osg::ref_ptr<osgViewer::Viewer> m_viewer;
    double m_mouseSensitivity;
    double m_movementSpeed;
    bool m_rightMouseButtonPressed;
    osg::Vec2d m_lastMousePosition;
    osg::Vec2f m_scenePosition;
    std::shared_ptr<Core::Event<int>> m_selectedObjectChangedEvent;
};

}