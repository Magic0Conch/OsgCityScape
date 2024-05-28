#include "GUI/Panels/Inspector.h"
#include "Core/ECS/Components/Mesh.h"
#include "Core/ECS/Components/Transform.h"
#include "Core/ECS/ObjectIDAllocator.h"
#include "Core/Event.h"
#include "Core/Helpers/VectorConverter.h"
#include "Editor/Core/RuntimeContext.h"
#include "Core/ECS/WorldManager.h"
#include "glm/fwd.hpp"
#include "imgui.h"
#include <functional>
#include <memory>
#include <string>
#include "GUI/Event/UIEventManager.h"
#include "osg/Math"
#include "osg/Matrix"
#include "osg/Matrixd"
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

void Inspector::onClickedIndexChanged(ECS::ObjectID clickedID){
    Core::g_runtimeContext.logSystem->info(std::to_string(clickedID) + "info show!");
    m_selectedObjectID = clickedID;
    m_curSceneObject = *m_objectsMap[m_selectedObjectID];
    auto& components = m_curSceneObject.getComponents();
    for (auto& [name,component] : components) {
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
            m_rotationEurler[0] = eurlerAngle.y / osg::PI * 180.0f;
            m_rotationEurler[1] = eurlerAngle.x / osg::PI * 180.0f;
            m_rotationEurler[2] = eurlerAngle.z / osg::PI * 180.0f;
        }
        if(name == "Mesh"){
            auto mesh = std::dynamic_pointer_cast<ECS::Mesh>(component);
            m_meshPath = mesh->getMeshPath();
        }
    }
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
        auto objectNameStr =  m_curSceneObject.getName();
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

        auto& components = m_curSceneObject.getComponents();
        for (auto& [name,component] : components) {
            if(name == "Transform"){
                if (ImGui::CollapsingHeader("Transform")){
                    ImGui::DragFloat3("Position", &m_position[0]);
                    ImGui::DragFloat3("Scale", &m_scale[0]);
                    auto transform = std::dynamic_pointer_cast<ECS::Transform>(component);    
                    if(m_useEuler^m_previousUseEuler){
                        if(m_previousUseEuler){
                            //to rotation
                            osg::Quat rotation;
                            rotation *= osg::Quat(m_rotationEurler[0] * osg::PI/180, osg::Vec3d(1, 0, 0));
                            rotation *= osg::Quat(m_rotationEurler[1] * osg::PI/180, osg::Vec3d(0, 1, 0));
                            rotation *= osg::Quat(m_rotationEurler[2] * osg::PI/180, osg::Vec3d(0, 0,1));               
                            m_rotation = Helpers::VectorConverter::osgVec4fToStdVector(rotation.asVec4());
                        }
                        else{
                            glm::quat q = glm::quat(m_rotation[3],m_rotation[0],m_rotation[1],m_rotation[2]);
                            glm::vec3 eurlerAngle = glm::eulerAngles(q);
                            m_rotationEurler[0] = eurlerAngle.x / osg::PI * 180.0f;
                            m_rotationEurler[1] = eurlerAngle.y / osg::PI * 180.0f;
                            m_rotationEurler[2] = eurlerAngle.z / osg::PI * 180.0f;
                        }
                        m_previousUseEuler = m_useEuler;
                    }                
                    if(m_useEuler){
                        osg::Quat rotation;
                        ImGui::DragFloat3("RotationEurler", &m_rotationEurler[0],1);
                        rotation *= osg::Quat(m_rotationEurler[0] * osg::PI/180, osg::Vec3d(1, 0, 0));
                        rotation *= osg::Quat(m_rotationEurler[1] * osg::PI/180, osg::Vec3d(0, 1, 0));
                        rotation *= osg::Quat(m_rotationEurler[2] * osg::PI/180, osg::Vec3d(0, 0,1));                        
                        transform->setRotation(rotation);
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
            if(name == "Mesh"){
                if (ImGui::CollapsingHeader("Mesh")){            
                    auto mesh = std::dynamic_pointer_cast<ECS::Mesh>(component);
                    ImGui::InputText("Mesh Path", &m_meshPath[0], 128);
                }
            }
        }

    }
    ImGui::End();
}