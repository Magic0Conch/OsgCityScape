#pragma once
#include "osg/Math"
#include <glm/fwd.hpp>


namespace CSEditor::Math {
    class Math{
    public:
        static double focal2fov(double focal)
        {
            if (focal < 0) focal = -focal;
            return  double(atan(1.0 / focal) * 2.0);
        }

        static double focal2fovEuler(double focal)
        {
            auto fov = focal2fov(focal);
            return  double(fov * 180.0 / osg::PI);
        }

    };

}