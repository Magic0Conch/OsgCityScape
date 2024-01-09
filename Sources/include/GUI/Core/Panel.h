#pragma once
#include <Windows.h>
#include <osgViewer/ViewerEventHandlers>

namespace CSEditor::GUI {
class Panel
{
public:

    void drawUi();
    virtual void drawImpl() = 0;
};
}
