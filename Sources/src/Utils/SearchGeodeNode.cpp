#include "Utils/SearchGeodeNode.h"

using namespace CSEditor::Utils;

void SearchGeodeNode::apply(osg::Node &searchNode){
    auto dynamicTry=dynamic_cast<osg::Geode*>(&searchNode);

    if(dynamicTry){
        if(m_useNameSearch){
            if(m_name==dynamicTry->getName())
                m_resultNode=dynamicTry;
        }
        else{
            m_resultNode=dynamicTry;
        }
    }
    traverse(searchNode);
}

osg::ref_ptr<osg::Geode> SearchGeodeNode::getNode() const{
    return m_resultNode;
}

SearchGeodeNode::SearchGeodeNode(const std::string& name):
osg::NodeVisitor(TRAVERSE_ALL_CHILDREN),m_name(name),m_useNameSearch(true){}

SearchGeodeNode::SearchGeodeNode():osg::NodeVisitor(TRAVERSE_ALL_CHILDREN){};