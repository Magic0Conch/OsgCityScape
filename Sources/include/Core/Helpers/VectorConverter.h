#pragma once
#include <vector>
#include <osg/Vec3f>
#include <osg/Vec4f>
namespace CSEditor::Helpers
{
class VectorConverter{
public:
    static std::vector<float> osgVec3fToStdVector(const osg::Vec3f& osgVector) {
        return {osgVector.ptr(), osgVector.ptr() + 3};
    }

    static osg::Vec3f stdVectorToOsgVec3f(const std::vector<float>& stdVector) {
        return osg::Vec3f(stdVector[0],stdVector[1],stdVector[2]);
    }
    
    static std::vector<float> osgVec4fToStdVector(const osg::Vec4f& osgVector) {
        return {osgVector.ptr(), osgVector.ptr() + 4};
    }

    static osg::Vec4f stdVectorToOsgVec4f(const std::vector<float>& stdVector) {
        return osg::Vec4f(stdVector[0],stdVector[1],stdVector[2],stdVector[3]);
    }
};
}