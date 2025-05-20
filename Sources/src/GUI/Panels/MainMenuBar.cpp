#include "GUI/Panels/MainMenuBar.h"
#include "Editor/Core/RuntimeContext.h"
#include "Utils/OsgconvWrapper.h"
#include "Windowing/Dialog/NfdDialog.h"
#include "Core/ECS/WorldManager.h"
#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"
#include "imgui_internal.h"
#include "Render/RenderSystem.h"
#include "Windowing/Window.h"
using namespace CSEditor::GUI;

MainMenuBar::MainMenuBar(){
    
}



void MainMenuBar::showImportWindow() {
    if (!m_showImportWindow) {
        return;
    }

    ImGui::Begin("Import Obj From Folder",&m_showImportWindow);

    ImGui::InputText("Input Folder Path", &m_inputFolderPath,1);
    if (ImGui::Button("Browse Input Folder")) {
        m_inputFolderPath = Dialog::NfdDialog::OpenFolderDlg();
    }

    ImGui::InputText("Output Folder Path", &m_outputFolderPath,1);
    if (ImGui::Button("Browse Output Folder")) {
        m_outputFolderPath = Dialog::NfdDialog::OpenFolderDlg();
    }

    if (ImGui::Button("Export")) {
        // auto activeLevel = Core::g_runtimeContext.worldManager->getCurrentActiveLevel();
        // activeLevel->importObjFromFolderRecursively(m_inputFolderPath);    
        Dialog::OsgconvWrapper::callOsgconvRecursively(m_inputFolderPath,m_outputFolderPath);

    }

    ImGui::End();
}

void MainMenuBar::showFlyToWindow() {
    if (!m_showFlyToWindow) {
        return;
    }

    ImGui::Begin("Fly To",&m_showFlyToWindow);
    ImGui::InputFloat3("Position (XYZ)", (float*)flyToPosition.ptr());
    ImGui::InputFloat3("Look at (XYZ)", (float*)lookAtPosition.ptr());

    if (ImGui::Button("Fly")) {
        auto camera = Core::g_runtimeContext.windowSystem->getMainCamera();
        camera->setViewMatrixAsLookAt(flyToPosition, lookAtPosition, osg::Vec3f(0, 0, 1));
        auto activeLevel = Core::g_runtimeContext.worldManager->getCurrentActiveLevel();
        // activeLevel->flyTo(osg::Vec3f(m_flyToX, m_flyToY, m_flyToZ));
    }

    ImGui::End();
}

void MainMenuBar::drawImpl(){
    showImportWindow();    
    showFlyToWindow();
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_MenuBar;
    bool p_open = true;
    ImGuiViewportP* viewport = (ImGuiViewportP*)(void*)ImGui::GetMainViewport();
    float height = ImGui::GetFrameHeight();
    if (ImGui::BeginViewportSideBar("##SecondaryMenuBar", viewport, ImGuiDir_Up, height, window_flags)) {
        if (ImGui::BeginMenuBar()) {
            if (ImGui::BeginMenu("File")){     
                if (ImGui::MenuItem("Convert Obj To ive")){
                    m_showImportWindow = true;
                }
                if (ImGui::MenuItem("import ive model")){
                    auto folderPath = Dialog::NfdDialog::OpenFolderDlg();
                    auto activeLevel = Core::g_runtimeContext.worldManager->getCurrentActiveLevel();
                    activeLevel->importObjFromFolderRecursively(folderPath);                
                }
                if (ImGui::MenuItem("import obj model")){
                    auto folderPath = Dialog::NfdDialog::OpenFolderDlg();
                    auto activeLevel = Core::g_runtimeContext.worldManager->getCurrentActiveLevel();
                    activeLevel->importObjFromFolderRecursively(folderPath,"obj");                
                }
                if (ImGui::MenuItem("Save As","CTRL + SHIFT + S")){
                    
                }
                if (ImGui::MenuItem("Capture Frame")){
                    Core::g_runtimeContext.renderSystem->captureFrame();

                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Edit")){
                if (ImGui::MenuItem("Load tile")){
                }
                if (ImGui::MenuItem("Fly to...")){
                    m_showFlyToWindow = true;
                }
                if(ImGui::MenuItem("Print Scene Graph")){
                    auto activeLevel = Core::g_runtimeContext.worldManager->getCurrentActiveLevel();
                    activeLevel->printSceneGraph();
                }
                if(ImGui::MenuItem("Test")){
                    Core::g_runtimeContext.assetManager->loadImage(R"(E:\work\2401\OsgCityScape\resources\textures\tryforfree_light.png)");
                }
                if (ImGui::BeginMenu("Add Object")){
                    if (ImGui::MenuItem("Add Empty Object")){
                        auto activeLevel = Core::g_runtimeContext.worldManager->getCurrentActiveLevel();
                        activeLevel->createObjectInLevel("Object",activeLevel->getRootObject()->getID());
                    }
                    if (ImGui::MenuItem("Add Camera")){
                    }
                    if (ImGui::MenuItem("Add Rectifier")){
                    }
                    if (ImGui::MenuItem("Light")){
                    }
                    if (ImGui::MenuItem("DatasetGen")){
                    }
                    ImGui::EndMenu();
                }
                if (ImGui::BeginMenu("Render Pipeline Mode")){
                    static int selectedOption = 0;

                    if (ImGui::MenuItem("Default", NULL, selectedOption == 0)) {
                        selectedOption = 0;
                        Core::g_runtimeContext.renderSystem->setRenderPipelineState(Render::RenderPipelineState::Default);
                    }
                    if (ImGui::MenuItem("TextureBaking", NULL, selectedOption == 1)) {
                        selectedOption = 1;
                        Core::g_runtimeContext.renderSystem->setRenderPipelineState(Render::RenderPipelineState::TextureBaking);
                    }
                    
                    ImGui::EndMenu();
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Assets")){
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Window")){
                ImGui::EndMenu();
            }
        
            ImGui::EndMenuBar();
        }
        ImGui::End();
    }
}
