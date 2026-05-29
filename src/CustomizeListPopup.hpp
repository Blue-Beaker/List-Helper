#pragma once

#include "main.hpp"
#include <Geode/ui/TextInput.hpp>

class CustomizeListPopup : public geode::Popup {
protected:
    TextInput* input;
    int m_levelId;
    GJGameLevel* m_level;

    bool setup(gd::string current);
    void onApply(CCObject*);
    void onCancel(CCObject*);

public:
    static CustomizeListPopup* create(gd::string current);
};
