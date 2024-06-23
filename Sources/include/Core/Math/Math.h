#pragma once
#include "osg/Math"
#include "osg/Quat"
#include "osg/Vec2"
#include "osg/Vec3"
#include "osg/Vec4"
#include <glm/fwd.hpp>
#include <vector>


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

        static osg::Vec3 rotateVector(const osg::Quat& q, const osg::Vec3& v) {
            osg::Quat vecQuat(v.x(), v.y(), v.z(),0.0f);
            osg::Quat resultQuat = q * vecQuat * q.conj();
            return osg::Vec3(resultQuat.x(), resultQuat.y(), resultQuat.z());
        }
        static std::vector<float> convertToVector(float val) {
            return {val};
        }
        static std::vector<float> convertToVector(const std::vector<float>& vec) {
            return vec;
        }
        static std::vector<float> convertToVector(const osg::Vec2& vec) {
            return {vec.x(), vec.y()};
        }
        static std::vector<float> convertToVector(const osg::Vec3& vec) {
            return {vec.x(), vec.y(), vec.z()};
        }

        static std::vector<float> convertToVector(const osg::Vec4& vec) {
            return {vec.x(), vec.y(), vec.z(), vec.w()};
        }
    };

}