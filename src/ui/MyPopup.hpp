#pragma once

#include "../main.hpp"

class MyPopup : public geode::Popup {
protected:
    cocos2d::CCArray* m_lists;
    geode::ScrollLayer* m_scrollLayer;
    int m_pickedIndex = -1;

    bool init(cocos2d::CCArray* lists);
    void rebuildList();
    void onMoveUp(cocos2d::CCObject* sender);
    void onMoveDown(cocos2d::CCObject* sender);
    void onPick(cocos2d::CCObject* sender);
    void onPlace(cocos2d::CCObject* sender);
    void onClose(cocos2d::CCObject* sender);

public:
    static MyPopup* create(cocos2d::CCArray* lists);
};
