#include "GUI/Helper/ImGuiInitOperation.h"

using namespace CSEditor::GUI;

ImGuiInitOperation::ImGuiInitOperation():osg::Operation("ImGuiInitOperation", false){}

void ImGuiInitOperation::operator()(osg::Object* object)
{
    osg::GraphicsContext* context = dynamic_cast<osg::GraphicsContext*>(object);
    if (!context)
        return;

    if (!ImGui_ImplOpenGL3_Init())
    {
        std::cout << "ImGui_ImplOpenGL3_Init() failed\n";
    }
}