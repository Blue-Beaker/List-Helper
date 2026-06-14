#pragma once

#include "ReorderListPopup.hpp"

class SortLevelsPopup : public ReorderListPopup {
protected:
    CCNode* setupRow(int index) override;
    void onClose(cocos2d::CCObject* sender) override;

public:
    static SortLevelsPopup* create(cocos2d::CCArray* lists, const std::string& title);
};
