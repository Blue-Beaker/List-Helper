#include "DictSortListPopup.hpp"
#include "../utils.hpp"

CCNode* DictSortListPopup::setupRow(int index) {
    auto list = static_cast<GJLevelList*>(m_lists->objectAtIndex(index));
    return createListRow(list);
}

gd::string DictSortListPopup::keyForObject(cocos2d::CCObject* obj, int index) const {
    // GD stores favorite lists with the listID converted to string as key.
    // We use m_listID to ensure the key is always correct.
    auto list = static_cast<GJLevelList*>(obj);
    return fmt::format("{}", list->m_listID);
}

DictSortListPopup* DictSortListPopup::create(cocos2d::CCDictionary* dict, const std::string& title) {
    if (!dict) return nullptr;

    auto ret = new DictSortListPopup();
    if (ret->init(dict, title)) {
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
}

void DictSortListPopup::onAfterClose(cocos2d::CCObject* sender) {
    // Refresh the LevelBrowserLayer to reflect reordered lists
    if (auto browserLayer = CCScene::get()->getChildByType<LevelBrowserLayer>(0)) {
        browserLayer->onRefresh(sender);
    }
}
