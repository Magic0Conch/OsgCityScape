// EditorInputManager.cpp
#include "Editor/Core/EditorInputManager.h"
#include "Core/EventManager.h"
#include "Editor/Core/RuntimeContext.h"
#include "Core/ECS/Level.h"
#include "Core/ECS/WorldManager.h"
#include "osg/Vec3d"
#include "spdlog/fmt/bundled/format.h"
#include "spdlog/spdlog.h"
#include <cmath>
#include <cstdlib>
#include <osg/Matrixd>
#include <osgFX/Outline>
#include <format>
#include "Windowing/Window.h"
#include <string>
#include "Core/EventManager.h"

using namespace CSEditor::Core;

EditorInputManager::EditorInputManager(osg::Camera* camera)
    : m_camera(camera), m_mouseSensitivity(0.002), m_movementSpeed(0.5), m_rightMouseButtonPressed(false) {
    g_runtimeContext.viewer->addEventHandler(this);
    m_viewer = g_runtimeContext.viewer;
    auto& selectedObjectChangedEvent = g_runtimeContext.eventManager->getOrCreateEvent<Core::Event<int>>("SelectedObjectChanged");
    m_selectedObjectChangedEvent.reset(&selectedObjectChangedEvent);

}

void EditorInputManager::setScenePosition(const osg::Vec2f& position){
    m_scenePosition = position;
}
osg::Vec2f EditorInputManager::getScenePosition() const{
    return m_scenePosition;
}


bool EditorInputManager::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa) {
    switch (ea.getEventType()) {
        case osgGA::GUIEventAdapter::PUSH:
            if (ea.getButton() == osgGA::GUIEventAdapter::RIGHT_MOUSE_BUTTON) {
                m_rightMouseButtonPressed = true;
                m_lastMousePosition.set(ea.getX(), ea.getY());
                return false; 
            }
            else if (ea.getButton() == osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON) {
                osgUtil::LineSegmentIntersector::Intersections intersections;
                osg::ref_ptr<osg::Node> node = new osg::Node();
                osg::ref_ptr<osg::Group> parent = new osg::Group();
                auto& log = Core::g_runtimeContext.logSystem;
                auto x = m_scenePosition.x();
                auto y = m_scenePosition.y();
                const auto& windowSize = Core::g_runtimeContext.windowSystem->getSize();
                x = x * windowSize.first;
                y = y * windowSize.second;
                osgUtil::Intersector::CoordinateFrame cf = osgUtil::Intersector::WINDOW;                
                log->info(fmt::format("Mouse Clicked at: {},{}",x,y));          
                if (x>0&&y>0&&m_viewer->computeIntersections(m_camera, cf,x, y,intersections))
                {
                    //得到选择的节点
                    osgUtil::LineSegmentIntersector::Intersection intersection = *intersections.begin();
                    osg::NodePath& nodePath = intersection.nodePath;	
                    node = nodePath.back();
                    auto drawable = intersection.drawable;
                    auto parents = drawable->getParents();
                    auto nodeToObjectID = g_runtimeContext.worldManager->getCurrentActiveLevel()->nodeToObjectID;
                    auto objectID = nodeToObjectID[node];
                    m_selectedObjectChangedEvent->invoke(objectID);
                    //点击节点切换高亮
                    auto parent = nodePath[nodePath.size() - 2];//当前选择节点的父节点
                    std::string ptrString = std::to_string(reinterpret_cast<uintptr_t>(node.get()));
                    std::string formattedString = fmt::format("Selected Node: {}, Ptr: {}, Position:{},{}.",objectID,ptrString,x,y);
                    log->info(formattedString);
                    
                    while(!parents.empty()){
                        auto node = parents.back();
                        parents.pop_back();
                    }
                    osg::ref_ptr<osg::Group> group =dynamic_cast<osg::Group*>( nodePath[2]);	
                    // osgFX::Outline *ot = dynamic_cast<osgFX::Outline*>(parent.get());
                    // if (!ot) //若ot不存在（未高亮） (node->parent)=>(node->outline->parent)
                    // {
                    //     osg::ref_ptr<osgFX::Outline> outline = new osgFX::Outline();
                    //     outline->setColor(osg::Vec4(1, 1, 0, 1));
                    //     outline->setWidth(5);
                    //     outline->addChild(node);
                    //     parent->replaceChild(node, outline);
                    // }
                    // //若ot存在（高亮）找出当前outline的父节点（node->outline->*itr）=>(node->*itr)
                    // else
                    // {
                    //     osg::Node::ParentList parentList = ot->getParents();
                    //     osg::Node::ParentList::iterator itr = parentList.begin();
                    //     (*itr)->replaceChild(ot, node);
                    // }
                }
                
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
                up = osg::Vec3d(0, 0, 1);
                osg::Vec3d lookDir = (center - eye);
                lookDir.normalize();
                osg::Vec3d sideDir = lookDir ^ up;
                sideDir.normalize();

                osg::Quat pitch(m_mouseSensitivity * mouseDelta.y(), sideDir);
                osg::Quat yaw(-m_mouseSensitivity * mouseDelta.x(), up);
                osg::Quat rotation = pitch * yaw;

                osg::Vec3d newLookDir =  rotation*lookDir;
                osg::Vec3d newUp = rotation*up;

                if(abs(newLookDir * up) >= 0.95){
                    return true;
                }
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
                eye -= up * m_movementSpeed;
                center -= up * m_movementSpeed;
                break;
            case osgGA::GUIEventAdapter::KEY_E:
                eye += up * m_movementSpeed;
                center += up * m_movementSpeed;
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