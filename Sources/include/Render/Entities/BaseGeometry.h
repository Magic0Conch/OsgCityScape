#ifndef BASEGEOMETRY_H
#define BASEGEOMETRY_H
#include <Windows.h>
#include <any>
#include <unordered_map>
#include "osg/Geometry"
#include "osg/RenderInfo"
namespace CSEditor::Render {
class BaseGeometry:public osg::Geometry{
public:
    BaseGeometry(int segments);
    void setSegments(float rhs);
    void setFloatProperty(const std::string& key,float value);
    void setIntProperty(const std::string& key,int value);
    std::unordered_map<std::string, std::any>& getProperties();
    template<class T>
    T getProperty(const std::string& key){
        return std::any_cast<T>(m_properties[key]);
    }
protected:
    bool m_isDirty = true;
    // int m_segments;
    osg::ref_ptr<osg::DrawElementsUInt> m_indices;

    std::unordered_map<std::string, std::any> m_properties;

    // void drawImplementation(osg::RenderInfo& renderInfo) const override;
private:
    virtual void update() = 0;
    class UpdateBaseGeometryCallback:public osg::NodeCallback{
    public:
        virtual void operator()(osg::Node* node,osg::NodeVisitor* nv);
    };
};
}

#endif