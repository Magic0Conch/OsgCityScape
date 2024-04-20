// EditorInputManager.h
#pragma once

#include "osg/ref_ptr"
#include "osgViewer/Viewer"
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
    void moveCamera(const osg::Vec3d& delta) ;
private:
    osg::ref_ptr<osg::Camera> m_camera;
    osg::ref_ptr<osgViewer::Viewer> m_viewer;
    double m_mouseSensitivity;
    double m_movementSpeed;
    bool m_rightMouseButtonPressed;
    osg::Vec2d m_lastMousePosition;
};

}