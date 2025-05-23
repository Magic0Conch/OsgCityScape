#pragma once
#include "Components/Component.h"
#include "Core/ECS/Components/Component.h"
#include "Core/ECS/Components/Transform.h"
#include "Core/ECS/ObjectIDAllocator.h"
#include "Resources/ResourceType/Common/Object.h"
#include "Core/ECS/Components/ComponentFactory.h"
#include "ObjectIDAllocator.h"
#include <memory>
#include <string>
#include <vector>

namespace CSEditor::ECS{
    class Object : public std::enable_shared_from_this<Object>{
    public:
        Object();
        Object(ObjectID id,ObjectID parentId);
        virtual ~Object();

        virtual void tick(float delta_time);

        bool load(const ResourceType::ObjectInstance& objectInstance);
        void save(ResourceType::ObjectInstance& outObjectInstance);

        bool hasComponent(const std::string& compenentTypename) const;
        std::vector<std::pair<std::string,std::shared_ptr<Component>>>& getComponents();

        
        template<typename T>
        std::shared_ptr<T> getComponent(){
            std::string tmpname=typeid(T).name();
            auto n = tmpname.rfind("::");
            auto componentType = tmpname.substr(n+2);
            for (auto& i:m_components){
                if (componentType==i.first||(componentType=="Mesh"&&(i.first=="ModelMesh"||i.first=="ProceduralMesh"))){
                    return std::static_pointer_cast<T>(i.second);
                }
            }       
            return nullptr;
        }
        
        template<class T,typename ... ArgsType>
        inline std::shared_ptr<T>& addComponent(ArgsType&&... args){
            std::string tmpname=typeid(T).name();
            auto n = tmpname.rfind("::");
            auto componentType = tmpname.substr(n+2);
            if(componentType == "Camera"){
                setShowTransform(false);
            }
            auto found = getComponent<T>();
            if(found)
                return found;
            std::shared_ptr<T> comp = ComponentFactory::createComponent<T>(std::forward<ArgsType>(args)...);
            m_components.push_back(
                std::make_pair(componentType, comp) 
            );
            comp->onComponentAdded(shared_from_this());
            return comp;
        }

        Transform& getTransformComponent();


        //getter and setter
        ObjectID getID() const;
        void setName(std::string name);
        void setTransform(Transform*);
        void setTransform(std::shared_ptr<Transform>);
        const std::string& getName() const;
        const std::string& getDefinitionUrl() const;
        void setDefinitionUrl(const std::string& definitionUrl);
        const ObjectID getParentId();
        void addChild(Object& childObject);
        void addChild(Transform& childTransform);
        const bool isLeaf();
        const std::vector<int>& getChildIndex() const;
        void setIsSelected(bool isSelected);
        const bool getIsSelected() const;
        void setShowTransform(bool showTransform);
        const bool getShowTransform() const;
    protected:
        ObjectID m_id ;
        std::string m_name;
        std::string m_definitionUrl;
        std::vector<std::pair<std::string,std::shared_ptr<Component>>> m_components;
        std::shared_ptr<Transform> m_transform;
        ObjectID m_parentId;
        bool m_isSelected = false;
        bool m_showTransform = true;
    };
} 
