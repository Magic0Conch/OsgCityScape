#pragma once
#include "Event.h"
#include "osgViewer/Viewer"
#include <map>
#include <string>
#include <variant>
namespace CSEditor::Core {
    class EventManager{
    using EventType = std::variant<Event<int,int>, Event<double>,Event<int>>;
    public:
        EventManager();
        void removeEvent(const std::string& eventName);        
        void setupEvents();
        
        template <typename T>
        T& getOrCreateEvent(const std::string& eventName) {
            if(m_eventMap.find(eventName) == m_eventMap.end()){
                m_eventMap.insert({eventName,T()});
            }
            auto& eventOriType = m_eventMap[eventName];
            auto& event = std::get<T>(eventOriType);
            return event;
        }

    private:        
        std::map<const std::string, EventType> m_eventMap;
        osg::ref_ptr<osgViewer::Viewer> m_viewer;
    };
}