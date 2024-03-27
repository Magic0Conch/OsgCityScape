#include "GUI/Panels/MainMenuBar.h"
#include "Editor/Core/RuntimeContext.h"
#include "Utils/OsgconvWrapper.h"
#include "Windowing/Dialog/NfdDialog.h"
#include "Core/ECS/WorldManager.h"
#include "misc/cpp/imgui_stdlib.h"


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

void MainMenuBar::drawImpl(){
    showImportWindow();
    if (ImGui::BeginMainMenuBar()){
        if (ImGui::BeginMenu("File")){     
            if (ImGui::MenuItem("Convert Obj To ive")){

                m_showImportWindow = true;
            }
            if (ImGui::MenuItem("import ive model")){
                auto folderPath = Dialog::NfdDialog::OpenFolderDlg();
                auto activeLevel = Core::g_runtimeContext.worldManager->getCurrentActiveLevel();
                activeLevel->importObjFromFolderRecursively(folderPath);                
            }
            if (ImGui::MenuItem("Save As","CTRL + SHIFT + S")){
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit")){
            if (ImGui::MenuItem("Load tile")){
            }
            if (ImGui::BeginMenu("Add Object")){
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
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Assets")){
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Window")){
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }

}
