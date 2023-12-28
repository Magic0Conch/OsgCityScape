#include "Transform.h"

namespace CSEditor::ECS {
    class ComponentFactory {
    public:
        static std::shared_ptr<Component> createComponent(const std::string& type);
    private:
        ComponentFactory() = default;
    };
}

