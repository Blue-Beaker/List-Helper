#pragma once

#include "../main.hpp"
#include <Geode/ui/ScrollLayer.hpp>

class ReorderListPopup : public geode::Popup {
protected:
    cocos2d::CCArray* m_lists;
    geode::ScrollLayer* m_scrollLayer;
    int m_pickedIndex = -1;
    std::string m_title;

    bool init(cocos2d::CCArray* lists, const std::string& title);
    void rebuildList();

    /// Subclasses must implement this to draw a row for the item at the given index.
    /// The row node should have content size { 250.f, 30.f }.
    virtual CCNode* setupRow(int index) = 0;

    virtual void onMoveUp(cocos2d::CCObject* sender);
    virtual void onMoveDown(cocos2d::CCObject* sender);
    virtual void onPick(cocos2d::CCObject* sender);
    virtual void onPlace(cocos2d::CCObject* sender);
};
