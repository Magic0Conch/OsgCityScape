#pragma once

#include "osg/Matrixd"
#include <osgFX/Export>
#include <osgFX/Effect>

namespace osgFX
{

    class OutlineFX : public Effect
    {
    public:
        /// Constructor.
        OutlineFX();

        /// Copy constructor.
        OutlineFX(const OutlineFX& copy, const osg::CopyOp& op = osg::CopyOp::SHALLOW_COPY) : Effect(copy, op) {
            _width = copy._width;
            _color = copy._color;
            _technique = copy._technique;
        }

        // Effect class info
        META_Effect(osgFX, OutlineFX, "Outline",
                    "Stencil buffer based object outline effect.\n"
                    "This effect needs a properly setup stencil buffer.",
                    "Ulrich Hertlein");

        /// Set outline width.
        void setWidth(float w);

        /// Get outline width.
        float getWidth() const {
            return _width;
        }

        /// Set outline color.
        void setColor(const osg::Vec4& color);
        void setProjectionMatrix(const osg::Matrixd& projectionMatrix);
        /// Get outline color.
        const osg::Vec4& getColor() const {
            return _color;
        }

    protected:
        /// Destructor.
        virtual ~OutlineFX() {
        }

        /// Define available techniques.
        bool define_techniques();

    private:
        /// Outline width.
        float _width;

        /// Outline color.
        osg::Vec4 _color;
        osg::Matrixd _projectionMatrix;

        /// Technique.
        class OutlineFXTechnique;
        OutlineFXTechnique* _technique;
    };

}
