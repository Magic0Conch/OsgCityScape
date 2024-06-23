#include "Windowing/Dialog/NfdDialog.h"
#include "nfd.h"
#include <spdlog/spdlog.h>
#include <iostream>
#include <string>
using namespace CSEditor::Dialog;

std::string NfdDialog::OpenFileDlg(){
    nfdchar_t* outPath = NULL;
    nfdresult_t result = NFD_OpenDialog(NULL,NULL, &outPath);
    if ( result == NFD_OKAY ) {
        auto filepth = std::string(outPath);
        free(outPath);
        outPath = nullptr;
        spdlog::info("[NFD] File Open Success! "+filepth);
        return filepth;
           
    }
    else if ( result == NFD_CANCEL ) {
        spdlog::info("[NFD] User pressed cancel.");
    }
    else {
        spdlog::error("[NFD] "+std::string(NFD_GetError()));
    }
    return "";
}

std::string NfdDialog::OpenFileDlg(const std::string& filter){
    nfdchar_t* outPath = nullptr;
    nfdresult_t result = NFD_OpenDialog(filter.c_str(),NULL, &outPath);
    if ( result == NFD_OKAY ) {
        auto filepth = std::string(outPath);
        free(outPath);
        outPath = nullptr;
        spdlog::info("[NFD] File Open Success! "+filepth);
        return filepth;
           
    }
    else if ( result == NFD_CANCEL ) {
        spdlog::info("[NFD] User pressed cancel.");
    }
    else {
        spdlog::error("[NFD] "+std::string(NFD_GetError()));
    }
    return "";
}

std::string NfdDialog::OpenFolderDlg(){
    nfdchar_t* outPath = NULL;
    nfdresult_t result = NFD_PickFolder(NULL, &outPath);
    if ( result == NFD_OKAY ) {
        auto folderpth = std::string(outPath);
        free(outPath);
        outPath = nullptr;
        spdlog::info("[NFD] Folder Open Success! "+folderpth);
        return folderpth;
           
    }
    else if ( result == NFD_CANCEL ) {
        spdlog::info("[NFD] User pressed cancel.");
    }
    else {
        spdlog::error("[NFD] "+std::string(NFD_GetError()));
    }
    return "";
}
