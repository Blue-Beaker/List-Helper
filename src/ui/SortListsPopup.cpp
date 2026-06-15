#include "SortListsPopup.hpp"
#include "../utils.hpp"

CCNode* SortListsPopup::setupRow(int index) {
    auto list = static_cast<GJLevelList*>(m_lists->objectAtIndex(index));
    return createListRow(list);
}

void SortListsPopup::onClose(CCObject* sender) {
    Popup::onClose(sender);
    // Refresh the LevelBrowserLayer to reflect reordered lists
    if (auto browserLayer = CCScene::get()->getChildByType<LevelBrowserLayer>(0)) {
        browserLayer->onRefresh(sender);
    }
}

SortListsPopup* SortListsPopup::create(cocos2d::CCArray* lists, const std::string& title) {
    auto ret = new SortListsPopup();
    if (ret->init(lists, title)) {
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
}
