
#include "Render/Effects/OutlineFX.h"
#include <osgFX/Registry>

#include <osg/Group>
#include <osg/Stencil>
#include <osg/CullFace>
#include <osg/PolygonMode>
#include <osg/LineWidth>
#include <osg/Texture1D>
#include "Resources/ResourceManagement/ConfigManager.h"
#include "Render/Pass/TextureProjectionPass.h"
#include "Editor/Core/RuntimeContext.h"
#include "Windowing/Window.h"
#include "Resources/Loaders/ShaderLoader.h"
#include "osg/Depth"

namespace
{
    const unsigned int Override_On = osg::StateAttribute::ON|osg::StateAttribute::OVERRIDE;
    const unsigned int Override_Off = osg::StateAttribute::OFF|osg::StateAttribute::OVERRIDE;
}


namespace osgFX
{
    /// Register prototype.
    Registry::Proxy proxy(new OutlineFX);

    class OutlineFX::OutlineFXTechnique : public Technique
    {
    public:
        /// Constructor.
        OutlineFXTechnique() : Technique(),
            _width(2), _color(1,1,1,1) {
        }

        /// Validate.
        bool validate(osg::State&) const {
            return true;
        }

        /// Set outline width.
        void setWidth(float w) {
            _width = w;
            auto uniform = outlineState->getUniform("_Outline");
            if(uniform)
                uniform->set(w);
        }

        /// Set outline color.
        void setColor(const osg::Vec4& color) {
            _color = color;
            auto uniform = outlineState->getUniform("_OutlineColor");
            if(uniform)
                uniform->set(color);
        }

    protected:
        /// Define render passes.
        void define_passes() {
            osg::ref_ptr<osg::Depth> depth = new osg::Depth;

            {
                osg::StateSet* state = new osg::StateSet;

                // stencil op
                osg::Stencil* stencil  = new osg::Stencil;
                stencil->setFunction(osg::Stencil::ALWAYS, 1, ~0u);
                stencil->setOperation(osg::Stencil::KEEP,
                                      osg::Stencil::KEEP,
                                      osg::Stencil::REPLACE);
                state->setAttributeAndModes(stencil, Override_On);
                depth->setFunction(osg::Depth::ALWAYS);
                state->setAttributeAndModes(depth, Override_On);
                const std::string& vertPath = (CSEditor::Core::g_runtimeContext.configManager->getShaderFolder() / "common/triangle.vert").string();
                const std::string& fragPath = (CSEditor::Core::g_runtimeContext.configManager->getShaderFolder() / "common/triangle.frag").string();
                osg::ref_ptr<osg::Program> program = CSEditor::Resources::ShaderLoader::create(vertPath, fragPath);
                state->setAttributeAndModes(program.get(), osg::StateAttribute::ON);
                addPass(state);
            }

            {
                outlineState = new osg::StateSet;

                // stencil op
                osg::Stencil* stencil  = new osg::Stencil;
                stencil->setFunction(osg::Stencil::NOTEQUAL, 1, ~0u);
                stencil->setOperation(osg::Stencil::KEEP,
                                      osg::Stencil::KEEP,
                                      osg::Stencil::REPLACE);
                outlineState->setAttributeAndModes(stencil, Override_On);
                outlineState->setAttributeAndModes(depth, Override_On);

                // disable modes
                outlineState->setMode(GL_BLEND, Override_Off);
                outlineState->addUniform(new osg::Uniform("_Outline", _width));
                outlineState->addUniform(new osg::Uniform("_OutlineColor", _color));

                const std::string& outlineVertPath = (CSEditor::Core::g_runtimeContext.configManager->getShaderFolder() / "localEffects/Outline.vert").string();
                const std::string& outlineFragPath = (CSEditor::Core::g_runtimeContext.configManager->getShaderFolder() / "localEffects/Outline.frag").string();
                osg::ref_ptr<osg::Program> outlineProgram = CSEditor::Resources::ShaderLoader::create(outlineVertPath, outlineFragPath);
                outlineState->setAttributeAndModes(outlineProgram.get(), osg::StateAttribute::ON);
                addPass(outlineState);
            }
        }

    private:
        float _width;
        osg::Vec4 _color;
        osg::StateSet* outlineState = new osg::StateSet;
    };

    OutlineFX::OutlineFX() : Effect(), _width(2), _color(1,1,1,1), _technique(0)
    {
    }

    void OutlineFX::setWidth(float w)
    {
        _width = w;
        if (_technique) {
            _technique->setWidth(w);
        }
    }

    void OutlineFX::setColor(const osg::Vec4& color)
    {
        _color = color;
        if (_technique) {
            _technique->setColor(color);
        }
    }

    bool OutlineFX::define_techniques()
    {
        _technique = new OutlineFXTechnique;
        addTechnique(_technique);

        setWidth(_width);
        setColor(_color);

        return true;
    }
}
