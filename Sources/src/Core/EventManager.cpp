#include "Core/EventManager.h"
#include "Editor/Core/RuntimeContext.h"
#include "Windowing/Window.h"
#include <unordered_map>
using namespace CSEditor::Core;
using EventType = std::variant<Event<int,int>, Event<double>>;

EventManager::EventManager(){
    //register common event
    auto& scenePanelSizeChangedevent = std::get<Core::Event<int,int>>(getOrCreateEvent("ScenePanelSizeChanged"));
    // scenePanelSizeChangedevent.addListener(g_runtimeContext.windowSystem->updateViewportSize);
    auto updateViewportSize = [](int width,int height){
        g_runtimeContext.windowSystem->updateViewportSize(width, height);
    };
    scenePanelSizeChangedevent.addListener(updateViewportSize);
    
    //register event to viewer
    m_viewer = g_runtimeContext.viewer;
    
}

EventType& EventManager::getOrCreateEvent(const std::string& eventName) {
    return m_eventMap[eventName];
}

void EventManager::removeEvent(const std::string& eventName) {
    m_eventMap.erase(eventName);
}
