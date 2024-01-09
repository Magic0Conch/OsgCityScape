#pragma once
#include "Components/Component.h"
#include "Core/ECS/ObjectIDAllocator.h"
#include "Resources/ResourceType/Common/Object.h"
#include "Core/ECS/Components/ComponentFactory.h"
#include "ObjectIDAllocator.h"
#include <memory>
#include <string>
#include <unordered_set>
#include <vector>

namespace CSEditor::ECS{
    class Object : public std::enable_shared_from_this<Object>{
    public:
        Object(ObjectID id,Transform& transform,ObjectID parentId);
        virtual ~Object();

        virtual void tick(float delta_time);

        bool load(const ResourceType::ObjectInstance& objectInstance);
        void save(ResourceType::ObjectInstance& outObjectInstance);

        bool hasComponent(const std::string& compenentTypename) const;
        std::vector<std::pair<std::string,std::shared_ptr<Component>>> getComponents();

        std::shared_ptr<Component> getComponent(const std::string& componentTypeName){
            for (auto& i:m_components){
                if (componentTypeName==i.first){
                    return i.second;
                }
            }       
            return nullptr;
        }
        
        template<class T,typename ... ArgsType>
        inline std::shared_ptr<T>& addComponent(ArgsType&&... args){
            std::string tmpname=typeid(T).name();
            auto n = tmpname.rfind("::");
            auto componentType = tmpname.substr(n+2);
            if (auto found = getComponent(componentType);found!=nullptr){
                T& instance = *dynamic_cast<T*>(found);
                return instance;
            }
            std::shared_ptr<T> comp = ComponentFactory::createComponent(componentType);
            m_components.push_back(
                std::make_pair(componentType, comp) 
            );
            comp->onComponentAdded();
            return comp;
        }

        Transform& getTransformComponent();


        //getter and setter
        ObjectID getID() const;
        void setName(std::string name);
        const std::string& getName() const;
        const std::string& getDefinitionUrl() const;
        void setDefinitionUrl(const std::string& definitionUrl);
        const ObjectID getParentId();

    protected:
        ObjectID m_id ;
        std::string m_name;
        std::string m_definitionUrl;
        std::vector<std::pair<std::string,std::shared_ptr<Component>>> m_components;
        Transform& m_transform;
        ObjectID m_parentId;
    };
} 
