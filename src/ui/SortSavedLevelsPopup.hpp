#pragma once

#include "../main.hpp"
#include <Geode/ui/ScrollLayer.hpp>

class SortSavedLevelsPopup : public geode::Popup {
protected:
    /// The current display order (indices into the original levels array)
    std::vector<int> m_order;

    /// The original levels array (not modified, retained)
    cocos2d::CCArray* m_levels;

    geode::ScrollLayer* m_scrollLayer;
    int m_pickedIndex = -1;

    bool init(cocos2d::CCArray* levels, const std::string& title);
    ~SortSavedLevelsPopup();
    void rebuildList();

    /// Subclasses can override to customize the row for the item at the given
    /// display index. The row node should have content size { 250.f, 30.f }.
    /// The base implementation adds name label, difficulty icon, and buttons.
    virtual CCNode* setupRow(int displayIndex);

    /// Called when building buttons for a row. Subclasses can override to
    /// customize the button menu. Default adds up/down and pick/place buttons.
    virtual CCMenu* setupButtonMenu(int displayIndex);

    void onMoveUp(cocos2d::CCObject* sender);
    void onMoveDown(cocos2d::CCObject* sender);
    void onPick(cocos2d::CCObject* sender);
    void onPlace(cocos2d::CCObject* sender);
    void onClose(cocos2d::CCObject* sender) override;

public:
    static SortSavedLevelsPopup* create(cocos2d::CCArray* levels, const std::string& title);
};
