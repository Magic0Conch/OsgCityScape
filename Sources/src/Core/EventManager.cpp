#include "Core/EventManager.h"
#include "Core/Event.h"
#include "Editor/Core/RuntimeContext.h"
#include "GUI/Panels/Hierachy.h"
#include "GUI/Panels/Inspector.h"
#include "Windowing/Window.h"
#include "Core/ECS/WorldManager.h"
#include "Core/ECS/Level.h"
#include "GUI/Core/UIManager.h"
#include <cstddef>
using namespace CSEditor::Core;
using EventType = std::variant<Event<int,int>, Event<double>,Event<int>>;

EventManager::EventManager(){
    m_viewer = g_runtimeContext.viewer;

}

void EventManager::setupEvents(){
    auto uiManager = Core::g_runtimeContext.uiManager;
    auto& uiHierachy = uiManager->getPanel<GUI::Hierachy>("Hierachy");
    auto& uiInspector = uiManager->getPanel<GUI::Inspector>("Inspector");
    //register common event
    auto& scenePanelSizeChangedevent = getOrCreateEvent<Event<int,int>>("ScenePanelSizeChanged");
    auto updateViewportSize = [](int width,int height){
        g_runtimeContext.windowSystem->updateViewportSize(width, height);
    };
    scenePanelSizeChangedevent.addListener(updateViewportSize);

    auto& selectedObjectChangedEvent = getOrCreateEvent<Core::Event<int>>("SelectedObjectChanged");
    auto updateSelectedObject = [&](int objectID){
        auto level = Core::g_runtimeContext.worldManager->getCurrentActiveLevel();
        level->setSelectedObjectID((std::size_t)objectID);
        uiInspector.onClickedIndexChanged(objectID);
        uiHierachy.setNodeClicked(objectID);
    };
    selectedObjectChangedEvent.addListener(updateSelectedObject);
}


void EventManager::removeEvent(const std::string& eventName) {
    m_eventMap.erase(eventName);
}
