#include "GUI/Panels/Inspector.h"
#include "Core/ECS/Components/ModelMesh.h"
#include "Core/ECS/Components/CompositeMesh/HighlightArea.h"
#include "Core/ECS/Components/Transform.h"
#include "Core/ECS/Components/Camera.h"
#include "Core/ECS/ObjectIDAllocator.h"
#include "Core/Helpers/VectorConverter.h"
#include "Editor/Core/RuntimeContext.h"
#include "Core/ECS/WorldManager.h"
#include "Render/Entities/BaseGeometry.h"
#include "Render/Entities/Circle.h"
#include "Render/Entities/Cylinder.h"
#include "glm/fwd.hpp"
#include "imgui.h"
#include <memory>
#include "Resources/ResourceManagement/ConfigManager.h"
#include <string>
#include "GUI/Event/UIEventManager.h"
#include "osg/Math"
#include "osg/ref_ptr"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>

using namespace CSEditor::GUI;

Inspector::Inspector():
m_level(Core::g_runtimeContext.worldManager->getCurrentActiveLevel()),
m_objectsMap(m_level->getSceneObjectsMap()),
m_position{0.0,0.0,0.0},
m_rotation{1.0,0.0,0.0,0.0},
m_rotationEurler{0,0,0},
m_scale{1.0,1.0,1.0},
m_meshPath(128,'\0'),
m_curSceneObject(){

}

std::vector<float> Inspector::eurlerToQuat(){
    osg::Quat rotation;
    rotation *= osg::Quat(m_rotationEurler[0] * osg::PI/180, osg::Vec3d(1, 0, 0));
    rotation *= osg::Quat(m_rotationEurler[1] * osg::PI/180, osg::Vec3d(0, 1, 0));
    rotation *= osg::Quat(m_rotationEurler[2] * osg::PI/180, osg::Vec3d(0, 0,1));               
    auto quat = Helpers::VectorConverter::osgVec4fToStdVector(rotation.asVec4());
    return quat;
}

std::vector<float> Inspector::quatToEurler(){
    glm::quat q = glm::quat(m_rotation[3],m_rotation[0],m_rotation[1],m_rotation[2]);
    glm::vec3 eurlerAngle = glm::eulerAngles(q);
    std::vector<float> rotationEurler(3);
    rotationEurler[0] = eurlerAngle.x / osg::PI * 180.0f;
    rotationEurler[1] = eurlerAngle.y / osg::PI * 180.0f;
    rotationEurler[2] = eurlerAngle.z / osg::PI * 180.0f;
    return rotationEurler;
}

void Inspector::onClickedIndexChanged(ECS::ObjectID clickedID){
    Core::g_runtimeContext.logSystem->info(std::to_string(clickedID) + "info show!");
    m_selectedObjectID = clickedID;
    m_curSceneObject = m_objectsMap[m_selectedObjectID];
    auto& components = m_curSceneObject->getComponents();
    for (auto& [name,component] : components) {\
        if(m_curSceneObject->getShowTransform()){
            if(name == "Transform"){
                auto transform = std::dynamic_pointer_cast<ECS::Transform>(component);                    
                auto position = Helpers::VectorConverter::osgVec3fToStdVector(transform->getPosition());
                m_position = position;
                auto rotation = Helpers::VectorConverter::osgVec4fToStdVector(transform->getRotation());
                m_rotation = rotation;
                auto scale = Helpers::VectorConverter::osgVec3fToStdVector(transform->getScale());
                m_scale = scale;

                glm::quat q = glm::quat(m_rotation[3],m_rotation[0],m_rotation[1],m_rotation[2]);
                glm::vec3 eurlerAngle = glm::eulerAngles(q);
                m_rotationEurler[0] = eurlerAngle.x / osg::PI * 180.0f;
                m_rotationEurler[1] = eurlerAngle.y / osg::PI * 180.0f;
                m_rotationEurler[2] = eurlerAngle.z / osg::PI * 180.0f;
            }
        }
        if(name == "ModelMesh"){
            auto mesh = std::dynamic_pointer_cast<ECS::ModelMesh>(component);
            m_meshPath = mesh->getMeshPath();
        }
        if(name == "Camera"){
            auto camera = std::dynamic_pointer_cast<ECS::Camera>(component);
            auto position = Helpers::VectorConverter::osgVec3fToStdVector(camera->getPosition());
            m_position = position;
            auto rotation = Helpers::VectorConverter::osgVec4fToStdVector(camera->getRotation());
            m_rotation = rotation;
            glm::quat q = glm::quat(m_rotation[3],m_rotation[0],m_rotation[1],m_rotation[2]);
            glm::vec3 eurlerAngle = glm::eulerAngles(q);
            m_rotationEurler[0] = eurlerAngle.x / osg::PI * 180.0f;
            m_rotationEurler[1] = eurlerAngle.y / osg::PI * 180.0f;
            m_rotationEurler[2] = eurlerAngle.z / osg::PI * 180.0f;
        }
    }
};

std::string matrixToString(const osg::Matrix& mat) {
    std::ostringstream oss;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            oss << mat(i, j);
            if (j < 3) {
                oss << ", ";
            }
        }
        if (i < 3) {
            oss << "\n";
        }
    }
    return oss.str();
}

void Inspector::drawImpl(){
    const auto main_viewport = ImGui::GetMainViewport();
    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_NoCollapse;
    ImGui::SetNextWindowPos(ImVec2(main_viewport->GetWorkCenter()),ImGuiCond_FirstUseEver);
    // ImGui::SetNextWindowSize(ImVec2(550,680),ImGuiCond_FirstUseEver);
    bool p_open = true;
    if(ImGui::Begin("Inspector",&p_open,window_flags)){
        if(m_selectedObjectID==-1||m_selectedObjectID==0){
            ImGui::End();
            return;
        }
        ImGui::SetNextItemWidth(ImGui::GetWindowWidth());
        auto objectNameStr =  m_curSceneObject->getName();
        auto objectNameCStr = const_cast<char*>(objectNameStr.c_str());
        
        static bool check = true;
        ImGui::AlignTextToFramePadding();
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 0));
        ImGui::Checkbox("##1", &check);
        ImGui::PopStyleVar(1);
        ImVec2 cursorPos = ImGui::GetCursorPos();
        ImGui::SameLine();
        ImGui::SetCursorPosY(cursorPos.y/2);

        ImGui::InputText("##2", objectNameCStr, 128,0,NULL,NULL);
        ImGui::Separator();

        auto& components = m_curSceneObject->getComponents();
        for (auto& [name,component] : components) {
            if(m_curSceneObject->getShowTransform()){

                if(name == "Transform"){
                    if (ImGui::CollapsingHeader("Transform") && m_curSceneObject->getShowTransform()){
                        ImGui::DragFloat3("Position", &m_position[0],0.1f);
                        ImGui::DragFloat3("Scale", &m_scale[0],0.1f);
                        auto transform = std::dynamic_pointer_cast<ECS::Transform>(component);    
                        if(m_useEuler^m_previousUseEuler){
                            if(m_previousUseEuler){
                                m_rotation = eurlerToQuat();
                            }
                            else{
                                m_rotationEurler = quatToEurler();
                            }
                            m_previousUseEuler = m_useEuler;
                        }
                        if(m_useEuler){
                            ImGui::DragFloat3("RotationEurler", &m_rotationEurler[0],1);
                            auto quatVec = eurlerToQuat();
                            osg::Quat quat = Helpers::VectorConverter::stdVectorToOsgVec4f(quatVec);                             
                            transform->setRotation(quat);
                        }
                        else {                    
                            ImGui::DragFloat4("Rotation", &m_rotation[0],0.05f);
                            auto rotation = Helpers::VectorConverter::stdVectorToOsgVec4f(m_rotation);
                            transform->setRotation(rotation);
                        }
                        ImGui::Checkbox("use eurler", &m_useEuler);
                        auto position = Helpers::VectorConverter::stdVectorToOsgVec3f(m_position);
                        transform->setPosition(position);
                        auto scale = Helpers::VectorConverter::stdVectorToOsgVec3f(m_scale);
                        transform->setScale(scale);
                    }
                }
            }
            if(name == "ModelMesh"){
                if (ImGui::CollapsingHeader("ModelMesh")){            
                    auto mesh = std::dynamic_pointer_cast<ECS::ModelMesh>(component);
                    ImGui::InputText("Mesh Path", &m_meshPath[0], 128);
                }
            }
            if(name == "Camera"){
                auto camera = std::dynamic_pointer_cast<ECS::Camera>(component);    
                if (ImGui::CollapsingHeader("Transform")){
                    DragFloatVectorWithSetter("Position", *camera, &ECS::Camera::getPositionVector, &ECS::Camera::setPositionVector, 0.1f);
                    if(m_useEuler^m_previousUseEuler){
                        if(m_previousUseEuler){
                            m_rotation = eurlerToQuat();
                        }
                        else{
                            m_rotationEurler = quatToEurler();
                        }
                        m_previousUseEuler = m_useEuler;
                    }
                    if(m_useEuler){
                        ImGui::DragFloat3("RotationEurler", &m_rotationEurler[0],1);
                        auto quatVec = eurlerToQuat();
                        osg::Quat quat = Helpers::VectorConverter::stdVectorToOsgVec4f(quatVec);
                        camera->setRotation(quat);
                    }
                    else {                    
                        ImGui::DragFloat4("Rotation", &m_rotation[0],0.05f);
                        auto rotation = Helpers::VectorConverter::stdVectorToOsgVec4f(m_rotation);
                        camera->setRotation(rotation);
                    }
                }
                if(ImGui::CollapsingHeader("Camera")){
                    DragFloatWithSetter("Field Of View", *camera, &ECS::Camera::getFieldOfView, &ECS::Camera::setFieldOfView, 0.1f);
                    DragFloatWithSetter("Near Clipping Plane", *camera, &ECS::Camera::getNearClippingPlane, &ECS::Camera::setNearClippingPlane, 0.1f);
                    DragFloatWithSetter("Far Clipping Plane", *camera, &ECS::Camera::getFarClippingPlane, &ECS::Camera::setFarClippingPlane, 0.1f);
                    InputTextWithSetter("Texture Path", *camera, &ECS::Camera::getProjectionTexturePath, &ECS::Camera::setProjectionTexturePath);
                    CheckBoxWithSetter("Projection Enabled", *camera, &ECS::Camera::getEnableProjectionTexture, &ECS::Camera::setEnableProjectionTexture);
                    if(ImGui::Button("output matrix")){
                        auto viewMatrix = camera->getCameraNode()->getViewMatrix();
                        auto projectionMatrix = camera->getCameraNode()->getProjectionMatrix();                        
                        spdlog::info("viewMatrix: {}",matrixToString(viewMatrix));
                        spdlog::info("projectionMatrix: {}",matrixToString(projectionMatrix));                        
                    }
                }
            }
            if(name == "HighlightArea"){
                component->drawImGui();
            }
        }
        if(ImGui::Button("Add Component")){
            ImGui::OpenPopup("AddComponentPopup");
        }
        if (ImGui::BeginPopup("AddComponentPopup")) {
            if (ImGui::Selectable("ModelMesh")) {
                using namespace CSEditor::ECS;
                const auto modelMeshPath = (Core::g_runtimeContext.configManager->getAssetFolder()/ "models/cube.obj").string();                
                m_curSceneObject->addComponent<ModelMesh>(modelMeshPath);
                onClickedIndexChanged(m_selectedObjectID);
            }
            if(ImGui::Selectable("HighlightArea")){
                m_curSceneObject->addComponent<ECS::HighlightArea>();
            }
            if (ImGui::Selectable("Camera")) {
                m_curSceneObject->addComponent<ECS::Camera>();
                onClickedIndexChanged(m_selectedObjectID);
            }
            ImGui::EndPopup();
        }
    }
    ImGui::End();
}