#include "GUI/Core/UIManager.h"
#include "Editor/Core/RuntimeContext.h"
#include "Windowing/Window.h"
#include "osgGA/GUIEventAdapter"
#include <imgui.h>
#include<iostream>
namespace CSEditor::GUI{

struct UIManager::ImGuiNewFrameCallback : public osg::Camera::DrawCallback
{
    ImGuiNewFrameCallback(UIManager& handler)
        : handler_(handler)
    {
    }

    void operator()(osg::RenderInfo& renderInfo) const override
    {
        handler_.newFrame(renderInfo);
    }

private:
    UIManager& handler_;
};

struct UIManager::ImGuiRenderCallback : public osg::Camera::DrawCallback
{
    ImGuiRenderCallback(UIManager& handler)
        : handler_(handler){
    }

    void operator()(osg::RenderInfo& renderInfo) const override{
        handler_.render(renderInfo);
        // auto contextId = renderInfo.getContextID();
        // auto textureObject = Core::g_runtimeContext.windowSystem->getScreenTexture()->getTextureObject(contextId);
        // auto id = textureObject->id();
        // std::cout<<id<<std::endl;
    }

private:
    UIManager& handler_;
};


UIManager::UIManager(): time_(0.0f), mousePressed_{false,false,false}, mouseWheel_(0.0f), initialized_(false){
    auto& eventOriType = Core::g_runtimeContext.eventManager->getOrCreateEvent("ScenePanelSizeChanged");
    auto& event = std::get<Core::Event<int,int>>(eventOriType);
    onScenePanelSizeChanged.reset(&event);        
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplOpenGL3_Init();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    initIO();
    
    ApplyStyle();
    // SetupFont();
    // SetupIconFont();
    // ImGui_ImplGlfw_InitForOpenGL(context.Wnd_p, true);
    // ImGui_ImplOpenGL3_Init(version.c_str());
    EnableDocking(true);
}

UIManager::~UIManager(){
    // ImGui_ImplOpenGL3_Shutdown();
	ImGui::DestroyContext();
}


void UIManager::setCameraCallbacks(osg::Camera* camera)
{
    camera->setPreDrawCallback(new ImGuiNewFrameCallback(*this));
    camera->setPostDrawCallback(new ImGuiRenderCallback(*this));
}

static int ConvertFromOSGKey(int key)
{
    using KEY = osgGA::GUIEventAdapter::KeySymbol;

    switch (key)
    {
        case KEY::KEY_Tab:
            return ImGuiKey_Tab;
        case KEY::KEY_Left:
            return ImGuiKey_LeftArrow;
        case KEY::KEY_Right:
            return ImGuiKey_RightArrow;
        case KEY::KEY_Up:
            return ImGuiKey_UpArrow;
        case KEY::KEY_Down:
            return ImGuiKey_DownArrow;
        case KEY::KEY_Page_Up:
            return ImGuiKey_PageUp;
        case KEY::KEY_Page_Down:
            return ImGuiKey_PageDown;
        case KEY::KEY_Home:
            return ImGuiKey_Home;
        case KEY::KEY_End:
            return ImGuiKey_End;
        case KEY::KEY_Delete:
            return ImGuiKey_Delete;
        case KEY::KEY_BackSpace:
            return ImGuiKey_Backspace;
        case KEY::KEY_Return:
            return ImGuiKey_Enter;
        case KEY::KEY_Escape:
            return ImGuiKey_Escape;
        default: // Not found
            return -1;
    }
}

void UIManager::initIO(){
    ImGuiIO& io = ImGui::GetIO();

    // Keyboard mapping. ImGui will use those indices to peek into the io.KeyDown[] array.
    // io.KeyMap[ImGuiKey_Tab] = ImGuiKey_Tab;
    // io.KeyMap[ImGuiKey_LeftArrow] = ImGuiKey_LeftArrow;
    // io.KeyMap[ImGuiKey_RightArrow] = ImGuiKey_RightArrow;
    // io.KeyMap[ImGuiKey_UpArrow] = ImGuiKey_UpArrow;
    // io.KeyMap[ImGuiKey_DownArrow] = ImGuiKey_DownArrow;
    // io.KeyMap[ImGuiKey_PageUp] = ImGuiKey_PageUp;
    // io.KeyMap[ImGuiKey_PageDown] = ImGuiKey_PageDown;
    // io.KeyMap[ImGuiKey_Home] = ImGuiKey_Home;
    // io.KeyMap[ImGuiKey_End] = ImGuiKey_End;
    // io.KeyMap[ImGuiKey_Delete] = ImGuiKey_Delete;
    // io.KeyMap[ImGuiKey_Backspace] = 523;
    // io.KeyMap[ImGuiKey_Enter] = ImGuiKey_Enter;
    // io.KeyMap[ImGuiKey_Escape] = ImGuiKey_Escape;
    // io.KeyMap[ImGuiKey_A] = osgGA::GUIEventAdapter::KeySymbol::KEY_A;
    // io.KeyMap[ImGuiKey_C] = osgGA::GUIEventAdapter::KeySymbol::KEY_C;
    // io.KeyMap[ImGuiKey_V] = osgGA::GUIEventAdapter::KeySymbol::KEY_V;
    // io.KeyMap[ImGuiKey_X] = osgGA::GUIEventAdapter::KeySymbol::KEY_X;
    // io.KeyMap[ImGuiKey_Y] = osgGA::GUIEventAdapter::KeySymbol::KEY_Y;
    // io.KeyMap[ImGuiKey_Z] = osgGA::GUIEventAdapter::KeySymbol::KEY_Z;
}
// void UIManager::SetupIconFont(){
//     ImGuiIO& io = ImGui::GetIO();
//     //io.Fonts->AddFontDefault();
//     float baseFontSize = baseFontSize_; // 13.0f is the size of the default font. Change to the font size you use.
//     float iconFontSize = baseFontSize * 2.0f / 3.0f; // FontAwesome fonts need to have their sizes reduced by 2.0f/3.0f in order to align correctly


//     // merge in icons from Font Awesome
//     // static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_16_FA, 0 };
//     ImFontConfig icons_config; 
//     icons_config.MergeMode = true; 
//     icons_config.PixelSnapH = true; 
//     icons_config.GlyphMinAdvanceX = iconFontSize;
//     // auto fullpth = ResourceManager::PathManager::GetFullPath(":fonts/"+std::string(FONT_ICON_FILE_NAME_FAS));
//     iconfont.small = io.Fonts->AddFontFromFileTTF( fullpth.c_str(), iconFontSize, &icons_config, icons_ranges );
//     iconfont.medium = io.Fonts->AddFontFromFileTTF( fullpth.c_str(), iconFontSize*2);
//     //use FONT_ICON_FILE_NAME_FAR if you want regular instead of solid
// }

// void UImanager::SetupFont(){
//     ImGuiIO& io = ImGui::GetIO();
//     //io.Fonts->AddFontDefault();
//     auto fullpth = ResourceManager::PathManager::GetFullPath(":fonts/SourceHanSansSC-Regular-2.otf");
//     textfont.small=io.Fonts->AddFontFromFileTTF( fullpth.c_str(), baseFontSize_,NULL,io.Fonts->GetGlyphRangesChineseFull());
//     //textfont.medium=io.Fonts->AddFontFromFileTTF( fullpth.c_str(), baseFontSize_*1.5);
//     //textfont.large=io.Fonts->AddFontFromFileTTF( fullpth.c_str(), baseFontSize_*2.25);
//     //ImGui::PushFont(textfont.small);
//     if (textfont.small==nullptr) {
//         textfont.small=io.Fonts->AddFontDefault();
//     }
//     io.FontDefault=textfont.small;
// }

void  UIManager::ApplyStyle()
{
	ImGuiStyle* style = &ImGui::GetStyle();

	// switch (p_style)
	// {
	// case OvUI::Styling::EStyle::IM_CLASSIC_STYLE:	ImGui::StyleColorsClassic();	break;
	// case OvUI::Styling::EStyle::IM_DARK_STYLE:		ImGui::StyleColorsDark();		break;
	// case OvUI::Styling::EStyle::IM_LIGHT_STYLE:		ImGui::StyleColorsLight();		break;
	// }
    //ImGui::StyleColorsDark();
    style->WindowMenuButtonPosition = -1;
	if (0)
	{
		style->WindowPadding = ImVec2(15, 15);
		style->WindowRounding = 5.0f;
		style->FramePadding = ImVec2(5, 5);
		style->FrameRounding = 4.0f;
		style->ItemSpacing = ImVec2(12, 8);
		style->ItemInnerSpacing = ImVec2(8, 6);
		style->IndentSpacing = 25.0f;
		style->ScrollbarSize = 15.0f;
		style->ScrollbarRounding = 9.0f;
		style->GrabMinSize = 5.0f;
		style->GrabRounding = 3.0f;

		style->Colors[ImGuiCol_Text] = ImVec4(0.80f, 0.80f, 0.83f, 1.00f);
		style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
		style->Colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
		style->Colors[ImGuiCol_ChildBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
		style->Colors[ImGuiCol_PopupBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
		style->Colors[ImGuiCol_Border] = ImVec4(0.2f, 0.2f, 0.2f, 0.88f);
		style->Colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
		style->Colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
		style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
		style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
		style->Colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
		style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.3f, 0.3f, 0.3f, 0.75f);
		style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
		style->Colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
		style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
		style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
		style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
		style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
		style->Colors[ImGuiCol_CheckMark] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
		style->Colors[ImGuiCol_SliderGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
		style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
		style->Colors[ImGuiCol_Button] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
		style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
		style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
		style->Colors[ImGuiCol_Header] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
		style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
		style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
		style->Colors[ImGuiCol_Separator] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
		style->Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
		style->Colors[ImGuiCol_SeparatorActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
		style->Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
		style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
		style->Colors[ImGuiCol_PlotLines] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
		style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
		style->Colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
		style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
		style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);
		

		style->Colors[ImGuiCol_Tab] = style->Colors[ImGuiCol_TabUnfocused];
	}
    else if (1)
    {
        style->WindowPadding = ImVec2(15, 15);
        style->WindowRounding = 0.0f;
        style->FramePadding = ImVec2(5, 5);
        style->FrameRounding = 0.0f;
        style->ItemSpacing = ImVec2(12, 8);
        style->ItemInnerSpacing = ImVec2(8, 6);
        style->IndentSpacing = 25.0f;
        style->ScrollbarSize = 15.0f;
        style->ScrollbarRounding = 0.0f;
        style->GrabMinSize = 5.0f;
        style->GrabRounding = 0.0f;
        style->TabRounding = 0.0f;
        style->ChildRounding = 0.0f;
        style->PopupRounding = 0.0f;

        style->WindowBorderSize = 1.0f;
        style->FrameBorderSize = 0.0f;
        style->PopupBorderSize = 1.0f;

        ImVec4* colors = ImGui::GetStyle().Colors;
        colors[ImGuiCol_Text] = ImVec4(0.96f, 0.96f, 0.99f, 1.00f);
        colors[ImGuiCol_TextDisabled] = ImVec4(0.0f, 0.50f, 0.50f, 1.00f);
        colors[ImGuiCol_WindowBg] = ImVec4(0.14f, 0.14f, 0.16f, 1.00f);
        colors[ImGuiCol_ChildBg] = ImVec4(0.09f, 0.09f, 0.10f, 1.00f);
        colors[ImGuiCol_PopupBg] = ImVec4(0.06f, 0.06f, 0.07f, 1.00f);
        colors[ImGuiCol_Border] = ImVec4(0.12f, 0.12f, 0.14f, 1.00f);
        colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_FrameBg] = ImVec4(0.06f, 0.06f, 0.07f, 1.00f);
        colors[ImGuiCol_FrameBgHovered] = ImVec4(0.20f, 0.20f, 0.22f, 1.00f);
        colors[ImGuiCol_FrameBgActive] = ImVec4(0.27f, 0.27f, 0.29f, 1.00f);
        colors[ImGuiCol_TitleBg] = ImVec4(0.07f, 0.07f, 0.07f, 1.00f);
        colors[ImGuiCol_TitleBgActive] = ImVec4(0.07f, 0.07f, 0.07f, 1.00f);
        colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.07f, 0.07f, 0.07f, 1.00f);
        colors[ImGuiCol_MenuBarBg] = ImVec4(0.07f, 0.07f, 0.07f, 1.00f);
        colors[ImGuiCol_ScrollbarBg] = ImVec4(0.07f, 0.07f, 0.07f, 1.00f);
        colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.32f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.42f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.53f, 1.00f);
        colors[ImGuiCol_CheckMark] = ImVec4(0.84f, 0.84f, 0.87f, 1.00f);
        colors[ImGuiCol_SliderGrab] = ImVec4(0.44f, 0.44f, 0.47f, 1.00f);
        colors[ImGuiCol_SliderGrabActive] = ImVec4(0.59f, 0.59f, 0.61f, 1.00f);
        colors[ImGuiCol_Button] = ImVec4(0.20f, 0.20f, 0.22f, 1.00f);
        colors[ImGuiCol_ButtonHovered] = ImVec4(0.44f, 0.44f, 0.47f, 1.00f);
        colors[ImGuiCol_ButtonActive] = ImVec4(0.59f, 0.59f, 0.61f, 1.00f);
        colors[ImGuiCol_Header] = ImVec4(0.20f, 0.20f, 0.22f, 1.00f);
        colors[ImGuiCol_HeaderHovered] = ImVec4(0.44f, 0.44f, 0.47f, 1.00f);
        colors[ImGuiCol_HeaderActive] = ImVec4(0.59f, 0.59f, 0.61f, 1.00f);
        colors[ImGuiCol_Separator] = ImVec4(1.00f, 1.00f, 1.00f, 0.20f);
        colors[ImGuiCol_SeparatorHovered] = ImVec4(0.44f, 0.44f, 0.47f, 0.39f);
        colors[ImGuiCol_SeparatorActive] = ImVec4(0.44f, 0.44f, 0.47f, 0.59f);
        colors[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.59f, 0.98f, 0.00f);
        colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.00f);
        colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.00f);
        colors[ImGuiCol_Tab] = ImVec4(0.20f, 0.20f, 0.22f, 1.00f);
        colors[ImGuiCol_TabHovered] = ImVec4(0.44f, 0.44f, 0.47f, 1.00f);
        colors[ImGuiCol_TabActive] = ImVec4(0.44f, 0.44f, 0.47f, 1.00f);
        colors[ImGuiCol_TabUnfocused] = ImVec4(0.20f, 0.20f, 0.22f, 0.39f);
        colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.44f, 0.44f, 0.47f, 0.39f);
        colors[ImGuiCol_DockingPreview] = ImVec4(0.91f, 0.62f, 0.00f, 0.78f);
        colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
        colors[ImGuiCol_PlotLines] = ImVec4(0.96f, 0.96f, 0.99f, 1.00f);
        colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.12f, 1.00f, 0.12f, 1.00f);
        colors[ImGuiCol_PlotHistogram] = ImVec4(0.96f, 0.96f, 0.99f, 1.00f);
        colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.12f, 1.00f, 0.12f, 1.00f);
        colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
        colors[ImGuiCol_DragDropTarget] = ImVec4(0.91f, 0.62f, 0.00f, 1.00f);
        colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
        colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
        colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
        colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
    }
}

void UIManager::EnableDocking(bool flg){
    dock_state_ = flg;
    if (flg) ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    else  ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_DockingEnable;
}

void UIManager::newFrame(osg::RenderInfo& renderInfo){
    // if (!panels_.size()) return;
    ImGui_ImplOpenGL3_NewFrame();
    ImGuiIO& io = ImGui::GetIO();
    
    auto viewport = Core::g_runtimeContext.windowSystem->getViewport();
    io.DisplaySize = ImVec2(viewport->width(), viewport->height());


    double currentTime = renderInfo.getView()->getFrameStamp()->getSimulationTime();
    io.DeltaTime = currentTime - time_ + 0.0000001;
    time_ = currentTime;
    for (int i = 0; i < 3; i++)
    {
        io.MouseDown[i] = mousePressed_[i];
    }
    io.MouseWheel = mouseWheel_;
    mouseWheel_ = 0.0f;
	ImGui::NewFrame();
    auto [x,y]  = ImGui::GetWindowSize();
    if (dockspace_on_){
        ImGuiViewport* viewport = ImGui::GetMainViewport();

        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("##dockspace", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking);
        ImGuiID dockspace_id = ImGui::GetID("MyDockspace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);



        ImGui::End();
        ImGui::PopStyleVar(3);      
    }
}

void UIManager::render(osg::RenderInfo&){
    for (auto& panel : panels_){
        panel->drawUi();
    }
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

bool UIManager::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
{
    if (!initialized_)
    {
        auto view = aa.asView();
        auto lastIndex = view->getNumSlaves() - 1;
        auto lastCamera = view->getSlave(lastIndex)._camera;
        if (view)
        {
            setCameraCallbacks(lastCamera);
            initialized_ = true;
        }
    }

    ImGuiIO& io = ImGui::GetIO();
    const bool wantCaptureMouse = io.WantCaptureMouse;
    const bool wantCaptureKeyboard = io.WantCaptureKeyboard;

    switch (ea.getEventType())
    {
        case osgGA::GUIEventAdapter::KEYDOWN:
        case osgGA::GUIEventAdapter::KEYUP:
        {
            const bool isKeyDown = ea.getEventType() == osgGA::GUIEventAdapter::KEYDOWN;
            const int c = ea.getKey();
            const int special_key = ConvertFromOSGKey(c);
            if (special_key > 0)
            {
                assert((special_key >= 0 && special_key < 645) && "ImGui KeysMap is an array of 645");
                io.KeysData[special_key].Down = isKeyDown;
                
                io.KeyCtrl = ea.getModKeyMask() & osgGA::GUIEventAdapter::MODKEY_CTRL;
                io.KeyShift = ea.getModKeyMask() & osgGA::GUIEventAdapter::MODKEY_SHIFT;
                io.KeyAlt = ea.getModKeyMask() & osgGA::GUIEventAdapter::MODKEY_ALT;
                io.KeySuper = ea.getModKeyMask() & osgGA::GUIEventAdapter::MODKEY_SUPER;
            }
            else if (isKeyDown && c > 0 && c < 0xFF)
            {
                io.AddInputCharacter((unsigned short)c);
            }
            return wantCaptureKeyboard;
        }
        case (osgGA::GUIEventAdapter::RELEASE):
        case (osgGA::GUIEventAdapter::PUSH):
        {
            io.MousePos = ImVec2(ea.getX(), io.DisplaySize.y - ea.getY());
            mousePressed_[0] = ea.getButtonMask() & osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON;
            mousePressed_[1] = ea.getButtonMask() & osgGA::GUIEventAdapter::RIGHT_MOUSE_BUTTON;
            mousePressed_[2] = ea.getButtonMask() & osgGA::GUIEventAdapter::MIDDLE_MOUSE_BUTTON;
            return wantCaptureMouse;
        }
        case (osgGA::GUIEventAdapter::DRAG):
        case (osgGA::GUIEventAdapter::MOVE):
        {
            io.MousePos = ImVec2(ea.getX(), io.DisplaySize.y - ea.getY());
            return wantCaptureMouse;
        }
        case (osgGA::GUIEventAdapter::SCROLL):
        {
            mouseWheel_ = ea.getScrollingMotion() == osgGA::GUIEventAdapter::SCROLL_UP ? 1.0 : -1.0;
            return wantCaptureMouse;
        }
        case (osgGA::GUIEventAdapter::RESIZE):
        {
            auto width = ea.getWindowWidth();
            auto height = ea.getWindowHeight();
            onScenePanelSizeChanged->invoke(width, height);
            return false;
        }
        default:
        {
            return false;
        }
    }

    return false;
}


}