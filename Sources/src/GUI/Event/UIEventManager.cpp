#include "GUI/Event/UIEventManager.h"

using namespace CSEditor::GUI;

UIEveneManager& UIEveneManager::getInstance(){
    static UIEveneManager _uiEveneManagerInstance;
    return _uiEveneManagerInstance;
}
