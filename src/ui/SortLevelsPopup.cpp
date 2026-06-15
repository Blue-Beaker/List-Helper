#include "SortLevelsPopup.hpp"
#include "../utils.hpp"

CCNode* SortLevelsPopup::setupRow(int index) {
    auto level = static_cast<GJGameLevel*>(m_lists->objectAtIndex(index));
    return createLevelRow(level);
}

void SortLevelsPopup::onClose(CCObject* sender) {
    Popup::onClose(sender);
    // Refresh the LevelBrowserLayer to reflect reordered levels
    if (auto browserLayer = CCScene::get()->getChildByType<LevelBrowserLayer>(0)) {
        browserLayer->onRefresh(sender);
    }
}

SortLevelsPopup* SortLevelsPopup::create(cocos2d::CCArray* levels, const std::string& title) {
    auto ret = new SortLevelsPopup();
    if (ret->init(levels, title)) {
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
}
