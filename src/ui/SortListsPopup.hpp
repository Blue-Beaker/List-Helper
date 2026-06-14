#pragma once

#include "ReorderListPopup.hpp"

class SortListsPopup : public ReorderListPopup {
protected:
    CCNode* setupRow(int index) override;
    std::string getTitle() const override;
    void onClose(cocos2d::CCObject* sender) override;

public:
    static SortListsPopup* create(cocos2d::CCArray* lists);
};
