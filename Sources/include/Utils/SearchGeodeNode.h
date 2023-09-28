#ifndef SearchGeodeNode_H
#define SearchGeodeNode_H
#include <windows.h>
#include <string>
#include <osg/Geode>
#include <osg/ref_ptr>
#include <osg/Program>
#include <osgDB/ReadFile>
#include <osgViewer/Viewer>
#include <osg/Camera>
#include <iostream>
namespace CSEditor::Utils{
class SearchGeodeNode:public osg::NodeVisitor{
public:
    SearchGeodeNode();
    SearchGeodeNode(const std::string& name);
    virtual void apply(osg::Node& searchNode) override;
    osg::ref_ptr<osg::Geode> getNode() const;
private:
    osg::ref_ptr<osg::Geode> m_resultNode;
    std::string m_name;
    bool m_useNameSearch = false;
};
}
#endif