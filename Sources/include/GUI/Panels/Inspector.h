#pragma once
#include <imgui.h>
#include <memory>
#include "Core/ECS/Level.h"
#include "Core/ECS/ObjectIDAllocator.h"
#include "GUI/Core/Panel.h"
#include "Core/ECS/Object.h"
namespace CSEditor::GUI {
class Inspector : public Panel{
public:
    Inspector();
    void onClickedIndexChanged(ECS::ObjectID clickedID);
protected:
    virtual void drawImpl() override;
private:
    std::vector<float> eurlerToQuat();
    std::vector<float> quatToEurler();

    std::shared_ptr<ECS::Level> m_level;
    std::unordered_map<ECS::ObjectID, std::shared_ptr<ECS::Object>>& m_objectsMap;
    ECS::ObjectID m_selectedObjectID = -1;
    std::vector<float> m_position;
    std::vector<float> m_rotation;
    std::vector<float> m_rotationEurler;
    std::vector<float> m_scale;
    std::string m_meshPath;
    std::shared_ptr<ECS::Object> m_curSceneObject;
    bool m_useEuler = false;
    bool m_previousUseEuler = false;
};
}
