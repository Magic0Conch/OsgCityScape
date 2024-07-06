#pragma once
#include <Windows.h>
#include <osgViewer/ViewerEventHandlers>
#include <imgui.h>
#include <string>
#include "Core/Math/Math.h"
#include "Windowing/Dialog/NfdDialog.h"

namespace CSEditor::GUI {
class Panel
{
public:
    template<typename T, typename Getter, typename Setter>
    void DragFloatWithSetter(const char* label, T& object, Getter getter, Setter setter,float step = 1.0f) {
        float tempValue = (object.*getter)();
        if (ImGui::DragFloat(label, &tempValue,step)) {
            (object.*setter)(tempValue);
        }
    }

    template<typename T, typename Getter, typename Setter>
    void CheckBoxWithSetter(const char* label, T& object, Getter getter, Setter setter) {
        bool tempValue = (object.*getter)();
        if (ImGui::Checkbox(label, &tempValue)) {
            (object.*setter)(tempValue);
        }
    }

    template<typename T, typename Getter, typename Setter>
    static void SliderFloatWithSetter(const char* label,const char* propertyName,T& object, Getter getter, Setter setter,float minRange = 0.0f,float maxRange = 1.0f) {
        float tempValue = (object.*getter)(propertyName);
        if (ImGui::SliderFloat(label, &tempValue, minRange, maxRange)) {
            (object.*setter)(propertyName,tempValue);
        }
    }
    
    template<typename T, typename Getter, typename Setter>
    static void SliderFloatWithSetter(const char* label,T& object, Getter getter, Setter setter,float minRange = 0.0f,float maxRange = 1.0f) {
        float tempValue = (object.*getter)();
        if (ImGui::SliderFloat(label, &tempValue, minRange, maxRange)) {
            (object.*setter)(tempValue);
        }
    }

    template<typename T, typename Getter, typename Setter>
    static void SliderIntWithSetter(const char* label,const char* propertyName,T& object, Getter getter, Setter setter,int minRange = 0,int maxRange = 1) {
        int tempValue = (object.*getter)(propertyName);
        if (ImGui::SliderInt(label, &tempValue, minRange, maxRange)) {
            (object.*setter)(propertyName,tempValue);
        }
    }

    template<typename T, typename Getter, typename Setter>
    static void SliderIntWithSetter(const char* label,T& object, Getter getter, Setter setter,int minRange = 0,int maxRange = 1) {
        int tempValue = (object.*getter)();
        if (ImGui::SliderInt(label, &tempValue, minRange, maxRange)) {
            (object.*setter)(tempValue);
        }
    }

    template<typename T, typename Getter, typename Setter>
    static void EditColorWithSetter(const char* label,const char* propertyName,T& object, Getter getter, Setter setter) {
        osg::Vec4 tempValue = (object.*getter)(propertyName);
        float color[4] = { tempValue.x(),tempValue.y(),tempValue.z(),tempValue.w() };
        if (ImGui::ColorEdit4(label, color)) {
            (object.*setter)(propertyName,osg::Vec4(color[0],color[1],color[2],color[3]));
        }
    }

    template<typename T, typename Getter, typename Setter>
    static void checkBoxWithSetter(const char* label,const char* propertyName, T& object, Getter getter, Setter setter) {
        bool tempValue = (object.*getter)(propertyName);
        if (ImGui::Checkbox(label, &tempValue)) {
            (object.*setter)(propertyName,tempValue);
        }
    }

    template<typename T, typename Getter, typename Setter>
    void DragFloatVectorWithSetter(const char* label, T& object, Getter getter, Setter setter,float step = 1.0f) {
        std::vector<float> tempValue = Math::Math::convertToVector((object.*getter)());
        switch (tempValue.size()) {
        case 2:
            if (ImGui::DragFloat2(label, &tempValue[0],step)) {
                (object.*setter)(tempValue);
            }
            break;
        case 3:
            if (ImGui::DragFloat3(label, &tempValue[0],step)) {
                (object.*setter)(tempValue);
            }
            break;
        case 4:
            if (ImGui::DragFloat4(label, &tempValue[0],step)) {
                (object.*setter)(tempValue);
            }
            break;
        }
    }

    template<typename T, typename Getter, typename Setter>
    void InputTextWithSetter(const char* label, T& object, Getter getter, Setter setter) {
        std::string tempValue = (object.*getter)();
        char buffer[128];
        strcpy(buffer, tempValue.c_str());
        float totalWidth = ImGui::CalcItemWidth();

        // 设置输入框宽度，使其加上按钮宽度等于总宽度
        float buttonWidth = ImGui::GetFrameHeight() + ImGui::GetStyle().ItemInnerSpacing.x;
        float inputTextWidth = totalWidth - buttonWidth;
        ImGui::PushItemWidth(inputTextWidth);
        if (ImGui::InputText("##hiddenLabel", buffer, 128)) {
            (object.*setter)(std::string(buffer));
        }
        ImGui::PopItemWidth();
        ImGui::SameLine();
        if (ImGui::Button("Browse", ImVec2(buttonWidth, 0))) {
            auto filePath = Dialog::NfdDialog::OpenFileDlg("jpg");
            if (filePath!="") {                
                strcpy(buffer, filePath.c_str());
                (object.*setter)(filePath);
            }
        }            
        ImGui::SameLine();
        ImGui::TextUnformatted(label);
    }
    void drawUi();
    virtual void drawImpl() = 0;
};
}
