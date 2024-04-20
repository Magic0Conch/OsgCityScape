#pragma once
#include "Event.h"
#include "osgViewer/Viewer"
#include <map>
#include <memory>
#include <string>
#include <variant>
namespace CSEditor::Core {
    class EventManager{
    using EventType = std::variant<Event<int,int>, Event<double>>;
    public:
        EventManager();
        EventType& getOrCreateEvent(const std::string& eventName);
        void removeEvent(const std::string& eventName);        
        

    private:        
        std::map<const std::string, EventType> m_eventMap;
        osg::ref_ptr<osgViewer::Viewer> m_viewer;
    };
}