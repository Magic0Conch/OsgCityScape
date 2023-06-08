#include <windows.h>
#include <iostream>
#include <osgViewer/Viewer>
#include <osgDB/ReadFile>
#include <string>


int main(){
    osgViewer::Viewer viewer;
    
    viewer.setSceneData(osgDB::readNodeFile("cow.osg"));    
    std::cout<<"Hello World!"<<std::endl;
    return viewer.run();
}