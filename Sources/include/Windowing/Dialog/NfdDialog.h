#pragma once
#include <string>
namespace CSEditor::Dialog{

class NfdDialog{
public:
static std::string OpenFileDlg();
static std::string OpenFolderDlg();
static std::string OpenFileDlg(const std::string& filter);
};

}