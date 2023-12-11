#include "Editor/Core/Application.h"
#include "osgDB/ReadFile"



int main(){
    CSEditor::Core::Application app;
    app.run();
    // 从文件加载模型
    osg::ref_ptr<osg::Node> model = osgDB::readNodeFile("path/to/your/model.osg");

    // 设置局部坐标，这里设置为平移
    osg::Matrix matrix;
    matrix.makeTranslate(osg::Vec3(2.0, 0.0, 0.0));

    // 直接在模型节点上设置局部坐标
    // model->setMatrix(matrix);
    return 0;
}