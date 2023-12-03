

#include "osg/GraphicsContext"
#include "osg/ref_ptr"
#include <memory>
namespace CSEditor::Core {
class RuntimeContext{
public:
    RuntimeContext();
    ~RuntimeContext();

    osg::ref_ptr<osg::GraphicsContext> graphicContext;
};
}