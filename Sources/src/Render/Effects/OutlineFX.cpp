
#include "Render/Effects/OutlineFX.h"
#include <osgFX/Registry>

#include <osg/Group>
#include <osg/Stencil>
#include <osg/CullFace>
#include <osg/PolygonMode>
#include <osg/LineWidth>
#include <osg/Material>
#include <osg/Texture1D>
#include "Resources/ResourceManagement/ConfigManager.h"
#include "Render/Pass/TextureProjectionPass.h"
#include "Editor/Core/RuntimeContext.h"
#include "Windowing/Window.h"
#include "Resources/Loaders/ShaderLoader.h"

namespace
{
    const unsigned int Override_On = osg::StateAttribute::ON|osg::StateAttribute::OVERRIDE;
    const unsigned int Override_Off = osg::StateAttribute::OFF|osg::StateAttribute::OVERRIDE;
}


namespace osgFX
{
    /// Register prototype.
    Registry::Proxy proxy(new OutlineFX);

    /**
     * Outline technique.
     */
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
            outlineState->getUniform("_Outline")->set(w);
        }

        /// Set outline color.
        void setColor(const osg::Vec4& color) {
            _color = color;
            outlineState->getUniform("_OutlineColor")->set(color);
        }

    protected:
        /// Define render passes.
        void define_passes() {

            /*
             * draw
             * - set stencil buffer to ref=1 where draw occurs
             * - clear stencil buffer to 0 where test fails
             */
            {
                osg::StateSet* state = new osg::StateSet;

                // stencil op
                osg::Stencil* stencil  = new osg::Stencil;
                stencil->setFunction(osg::Stencil::ALWAYS, 1, ~0u);
                stencil->setOperation(osg::Stencil::KEEP,
                                      osg::Stencil::KEEP,
                                      osg::Stencil::REPLACE);
                state->setAttributeAndModes(stencil, Override_On);
                const std::string& vertPath = (CSEditor::Core::g_runtimeContext.configManager->getShaderFolder() / "common/triangle.vert").string();
                const std::string& fragPath = (CSEditor::Core::g_runtimeContext.configManager->getShaderFolder() / "common/triangle.frag").string();
                osg::ref_ptr<osg::Program> program = CSEditor::Resources::ShaderLoader::create(vertPath, fragPath);                   
                state->setAttributeAndModes(program.get(), osg::StateAttribute::ON);
                addPass(state);
            }

            /*
             * post-draw
             * - only draw where draw didn't set the stencil buffer
             * - draw only back-facing polygons
             * - draw back-facing polys as lines
             * - disable depth-test, lighting & texture
             */
            {
                outlineState = new osg::StateSet;

                // stencil op
                osg::Stencil* stencil  = new osg::Stencil;
                stencil->setFunction(osg::Stencil::NOTEQUAL, 1, ~0u);
                stencil->setOperation(osg::Stencil::KEEP,
                                      osg::Stencil::KEEP,
                                      osg::Stencil::REPLACE);
                outlineState->setAttributeAndModes(stencil, Override_On);

                // cull front-facing polys
                osg::CullFace* cullFace = new osg::CullFace;
                cullFace->setMode(osg::CullFace::FRONT);
                outlineState->setAttributeAndModes(cullFace, Override_On);

                // disable modes
                outlineState->setMode(GL_BLEND, Override_Off);
                //state->setMode(GL_DEPTH_TEST, Override_Off);
                outlineState->setTextureMode(0, GL_TEXTURE_1D, Override_Off);
                outlineState->setTextureMode(0, GL_TEXTURE_2D, Override_Off);
                outlineState->setTextureMode(0, GL_TEXTURE_3D, Override_Off);
                outlineState->addUniform(new osg::Uniform("_Outline", _width));
                outlineState->addUniform(new osg::Uniform("_OutlineColor", _color));

                const std::string& vertPath = (CSEditor::Core::g_runtimeContext.configManager->getShaderFolder() / "localEffects/Outline.vert").string();
                const std::string& fragPath = (CSEditor::Core::g_runtimeContext.configManager->getShaderFolder() / "localEffects/Outline.frag").string();
                osg::ref_ptr<osg::Program> program = CSEditor::Resources::ShaderLoader::create(vertPath, fragPath);
                outlineState->setAttributeAndModes(program.get(), osg::StateAttribute::ON);
                addPass(outlineState);
            }
        }

    private:
        float _width;
        osg::Vec4 _color;
        osg::StateSet* outlineState = new osg::StateSet;
    };


    /**
     * Outline effect.
     */
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
