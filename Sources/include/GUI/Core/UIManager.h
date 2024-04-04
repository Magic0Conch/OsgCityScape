#pragma once
#include <string>
#include <vector>
#include <memory>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include "Panel.h"
#include <unordered_map>
#include "Core/EventManager.h"
namespace osg {
class Camera;
}


namespace CSEditor::GUI{

// struct LevelFonts{
//     ImFont* small;
//     ImFont* medium;
//     ImFont* large;
// };

class UIManager: public osgGA::GUIEventHandler{
public:
    UIManager();
    ~UIManager();
    
    template<typename T,typename... Args>
    void createPanel(const std::string& id,Args&&... args){
        panels_.push_back(std::make_unique<T>(std::forward<Args>(args)...));
        //if constexpr (std::is_base_of<SEditor::Panels::SView,T>::value){
        panelmap_.emplace(id,panels_.size()-1);
        //}
        
    }
    template<typename T>
    T& getPanel(const std::string& id){
        return *static_cast<T*>(panels_[panelmap_[id]].get());
    }
    // void SetupIconFont();
    // void SetupFont();
    void ApplyStyle();
    void newFrame(osg::RenderInfo& renderInfo);
    void render(osg::RenderInfo&);
    void EnableDocking(bool flg);
    bool dock_state_;
    bool dockspace_on_=true;
    float baseFontSize_=15.0f;
    std::vector <std::unique_ptr<Panel>> panels_;
    std::unordered_map <std::string,int> panelmap_;
    bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa) override;
    void setCameraCallbacks(osg::Camera* camera);

    void initIO();
private:
    struct ImGuiNewFrameCallback;
    struct ImGuiRenderCallback;

    double time_;
    bool mousePressed_[3];
    float mouseWheel_;
    bool initialized_;
    std::shared_ptr<Core::Event<int,int>> onScenePanelSizeChanged;
};

}