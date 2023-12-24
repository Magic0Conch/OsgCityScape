#pragma once
#include "Components/Component.h"
#include "Resources/ResourceType/Common/Object.h"
#include "ObjectIDAllocator.h"
#include <memory>
#include <string>
#include <unordered_set>
#include <vector>

namespace CSEditor::ECS{
    class Object : public std::enable_shared_from_this<Object>{
    public:
        Object(ObjectID id);
        virtual ~Object();

        virtual void tick(float delta_time);

        bool load(const ResourceType::ObjectInstance& objectInstance);
        void save(ResourceType::ObjectInstance& outObjectInstance);

        bool hasComponent(const std::string& compenentTypename) const;
        std::vector<Component> getComponents();
        
        //getter and setter
        ObjectID getID() const;
        void setName(std::string name);
        const std::string& getName() const;
        const std::string& getDefinitionUrl() const;
        void setDefinitionUrl(const std::string& definitionUrl);        

    protected:
        ObjectID m_id ;
        std::string m_name;
        std::string m_definitionUrl;
        std::vector<Component> m_components;
    };
} 
