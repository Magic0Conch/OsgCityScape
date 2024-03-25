#include "GUI/Panels/MainMenuBar.h"
#include "Editor/Core/RuntimeContext.h"
#include "Windowing/Dialog/NfdDialog.h"
#include "Core/ECS/WorldManager.h"

using namespace CSEditor::GUI;

MainMenuBar::MainMenuBar(){
    
}

void MainMenuBar::drawImpl(){
    if (ImGui::BeginMainMenuBar()){
        if (ImGui::BeginMenu("File")){     
            if (ImGui::MenuItem("Import Obj From Folder")){
                auto folderPath = Dialog::NfdDialog::OpenFolderDlg();
                auto activeLevel = Core::g_runtimeContext.worldManager->getCurrentActiveLevel();
                activeLevel->importObjFromFolderRecursively(folderPath);                
            }
            if (ImGui::MenuItem("open","O")){
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
