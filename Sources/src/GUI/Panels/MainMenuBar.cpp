#include "GUI/Panels/MainMenuBar.h"

using namespace CSEditor::GUI;

void MainMenuBar::drawImpl(){
    if (ImGui::BeginMainMenuBar()){
        if (ImGui::BeginMenu("File")){            
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
