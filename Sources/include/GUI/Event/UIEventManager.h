#include "Core/ECS/ObjectIDAllocator.h"
#include "Core/Event.h"

namespace CSEditor::GUI {
class UIEveneManager{
public:
    static UIEveneManager& getInstance();
    Core::Event<ECS::ObjectID> selectHierachyEvent;
};
}