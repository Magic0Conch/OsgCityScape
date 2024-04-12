#pragma once
#include "Editor/Core/RuntimeContext.h"
#include "Resources/ResourceManagement/ConfigManager.h"
#include "osgDB/FileNameUtils"
#include "osgDB/FileUtils"
#include <cstdlib>
#include <string>
#include <iostream>
#include <thread>
#include <filesystem>

namespace CSEditor::Dialog{
class OsgconvWrapper {
public:
    static void callOsgconv(const std::string& inputPath, const std::string& outputPath) {
        std::string command = (Core::g_runtimeContext.configManager->getBinaryFolder() /"osgconv.exe ").string()+ inputPath + " " + outputPath;
        std::cout<<command<<std::endl;
        // E:\work\2401\OpenSceneGraph\bin
        command = "E:\\work\\2401\\OpenSceneGraph\\bin\\osgconv.exe "+ inputPath + " " + outputPath;
        int result = std::system(command.c_str());
        if (result != EXIT_SUCCESS) {
            std::cerr << "osgconv.exe failed with error code: " << result << std::endl;
        }
    }

    static bool callOsgconvRecursively(const std::string& inputPath,const std::string& outputPath){
        osgDB::DirectoryContents contents = osgDB::getDirectoryContents(inputPath);
        for (const auto& file : contents) {
            std::string fullPath = osgDB::concatPaths(inputPath, file);
            if (osgDB::fileType(fullPath) == osgDB::DIRECTORY && file!="." && file!="..") {
                callOsgconvRecursively(fullPath,outputPath);
            } else if (osgDB::getLowerCaseFileExtension(fullPath) == "obj") {
                if (fullPath.find("_bbox") != std::string::npos) {
                    continue;
                }
                std::filesystem::path pathObj(fullPath);
                std::string fileName = pathObj.filename().replace_extension("ive").string();
                std::string newPath = outputPath + "/" + fileName;
                std::cout << "start conv: "<<fullPath << std::endl;
                callOsgconvAsync(fullPath, newPath);
            }
        }

        return true;
    }

    static void callOsgconvAsync(const std::string& inputPath, const std::string& outputPath) {
        std::thread workerThread(callOsgconv, inputPath, outputPath);
        workerThread.detach();
    }
};
}