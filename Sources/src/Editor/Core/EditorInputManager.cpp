// EditorInputManager.cpp
#include "Editor/Core/EditorInputManager.h"
#include "Editor/Core/RuntimeContext.h"
#include "osg/Vec3d"
#include <osg/Matrixd>

using namespace CSEditor::Core;

EditorInputManager::EditorInputManager(osg::Camera* camera)
    : m_camera(camera), m_mouseSensitivity(0.002), m_movementSpeed(0.5), m_rightMouseButtonPressed(false) {
    g_runtimeContext.viewer->addEventHandler(this);

}

bool EditorInputManager::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa) {
    switch (ea.getEventType()) {
        case osgGA::GUIEventAdapter::PUSH:
            if (ea.getButton() == osgGA::GUIEventAdapter::RIGHT_MOUSE_BUTTON) {
                m_rightMouseButtonPressed = true;
                m_lastMousePosition.set(ea.getX(), ea.getY());
                return false; 
            }
            break;
        case osgGA::GUIEventAdapter::RELEASE:
            if (ea.getButton() == osgGA::GUIEventAdapter::RIGHT_MOUSE_BUTTON) {
                m_rightMouseButtonPressed = false;
                return false; 
            }
            break;
        case osgGA::GUIEventAdapter::DRAG:
            if (m_rightMouseButtonPressed) {
                osg::Vec2d mouseDelta(ea.getX() - m_lastMousePosition.x(), ea.getY() - m_lastMousePosition.y());
                m_lastMousePosition.set(ea.getX(), ea.getY());
                
                osg::Vec3d eye, center, up;
                m_camera->getViewMatrixAsLookAt(eye, center, up);
                osg::Vec3d lookDir = center - eye;
                osg::Vec3d sideDir = lookDir ^ up;

                osg::Quat pitch(-m_mouseSensitivity * mouseDelta.y(), sideDir);
                osg::Quat yaw(-m_mouseSensitivity * mouseDelta.x(), up);
                osg::Quat rotation = pitch * yaw;

                // osg::Matrixd rotationMatrix;
                // rotation.get(rotationMatrix);
                
                osg::Vec3d newLookDir =  rotation*lookDir;
                osg::Vec3d newUp = rotation*up;

                m_camera->setViewMatrixAsLookAt(eye, eye + newLookDir, newUp);

                return true; // 返回true表示事件已被处理
            }
            break;
        case osgGA::GUIEventAdapter::SCROLL:
            if (ea.getScrollingMotion() == osgGA::GUIEventAdapter::SCROLL_UP) {
                if(m_movementSpeed<2.0){
                    m_movementSpeed *= 1.1;
                }
            } else if (ea.getScrollingMotion() == osgGA::GUIEventAdapter::SCROLL_DOWN) {
                if(m_movementSpeed>0.1){
                    m_movementSpeed /= 1.1;
                }
            }
            return true; // 返回true表示事件已被处理
        default:
            break;
    }

    // 处理wasd键盘移动
     if (ea.getEventType() == osgGA::GUIEventAdapter::KEYDOWN) {
        osg::Vec3d eye, center, up;
        m_camera->getViewMatrixAsLookAt(eye, center, up);
        osg::Vec3d lookDir = center - eye;
        osg::Vec3d sideDir = lookDir ^ up;

        switch (ea.getKey()) {
            case osgGA::GUIEventAdapter::KEY_W:
                // 向前移动
                eye += lookDir * m_movementSpeed;
                center += lookDir * m_movementSpeed;
                break;
            case osgGA::GUIEventAdapter::KEY_S:
                // 向后移动
                eye -= lookDir * m_movementSpeed;
                center -= lookDir * m_movementSpeed;
                break;
            case osgGA::GUIEventAdapter::KEY_A:
                // 向左移动
                eye -= sideDir * m_movementSpeed;
                center -= sideDir * m_movementSpeed;
                break;
            case osgGA::GUIEventAdapter::KEY_D:
                // 向右移动
                eye += sideDir * m_movementSpeed;
                center += sideDir * m_movementSpeed;
                break;
            case osgGA::GUIEventAdapter::KEY_Q:
                eye += up * m_movementSpeed;
                center += up * m_movementSpeed;
                break;
            case osgGA::GUIEventAdapter::KEY_E:
                eye -= up * m_movementSpeed;
                center -= up * m_movementSpeed;
                break;
            
            default:
                return false;
        }
        m_camera->setViewMatrixAsLookAt(eye, center, up);
        return true;
    }

    return false;
}

void EditorInputManager::setMouseSensitivity(double sensitivity) {
    m_mouseSensitivity = sensitivity;
}

void EditorInputManager::setMovementSpeed(double speed) {
    m_movementSpeed = speed;
}

void EditorInputManager::moveCamera(const osg::Vec3d& delta) {
    osg::Vec3d eye, center, up;
    m_camera->getViewMatrixAsLookAt(eye, center, up);

    osg::Vec3d lookDir = center - eye;
    osg::Vec3d sideDir = lookDir ^ up;
    osg::Vec3d newEye = eye + sideDir * delta.x() + up * delta.y() + lookDir * delta.z();
    osg::Vec3d newCenter = center + sideDir * delta.x() + up * delta.y() + lookDir * delta.z();

    m_camera->setViewMatrixAsLookAt(newEye, newCenter, up);
}