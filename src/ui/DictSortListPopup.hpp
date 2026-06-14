#pragma once

#include "ReorderDictPopup.hpp"

class DictSortListPopup : public ReorderDictPopup {
protected:
    CCNode* setupRow(int index) override;
    gd::string keyForObject(cocos2d::CCObject* obj, int index) const override;
    void onAfterClose(cocos2d::CCObject* sender) override;

public:
    static DictSortListPopup* create(cocos2d::CCDictionary* dict, const std::string& title);
};
