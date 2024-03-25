#pragma once
#include "Editor/Core/RuntimeContext.h"
#include "Resources/ResourceManagement/ConfigManager.h"
#include <cstdlib>
#include <string>
#include <iostream>
#include <thread>
namespace CSEditor::Dialog{
class OsgconvWrapper {
public:
    static void callOsgconv(const std::string& inputPath, const std::string& outputPath) {
        std::string command = (Core::g_runtimeContext.configManager->getBinaryFolder() /"osgconv.exe ").string()+ inputPath + " " + outputPath;
        std::cout<<command<<std::endl;
        int result = std::system(command.c_str());
        if (result != EXIT_SUCCESS) {
            std::cerr << "osgconv.exe failed with error code: " << result << std::endl;
        }
    }

    static void callOsgconvAsync(const std::string& inputPath, const std::string& outputPath) {
        // 在新的线程中执行osgconv.exe
        std::thread workerThread(callOsgconv, inputPath, outputPath);

        // 分离线程以便它可以在后台运行
        workerThread.detach();
    }
};
}