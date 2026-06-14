#include "DictSortListPopup.hpp"
#include "../utils.hpp"

std::string DictSortListPopup::getTitle() const {
    return "Sort Favorite Lists";
}

CCNode* DictSortListPopup::setupRow(int index) {
    auto list = static_cast<GJLevelList*>(m_lists->objectAtIndex(index));
    if (!list) return CCNode::create();
    auto row = CCNode::create();
    row->setContentSize({ 250.f, 30.f });

    // List name label on the left
    auto nameLabel = CCLabelBMFont::create(
        list->m_listName.c_str(), "bigFont.fnt"
    );
    nameLabel->setScale(0.35f);
    nameLabel->setAnchorPoint(ccp(0, 0.5f));
    nameLabel->setPosition(ccp(28, 15.f));
    nameLabel->setID("name-label");
    row->addChild(nameLabel);

    // Difficulty icon using the list's difficulty frame
    auto diffFrame = GJLevelList::frameForListDifficulty(list->m_difficulty, DifficultyIconType::ShortText);
    auto diffSprite = CCSprite::createWithSpriteFrameName(diffFrame.c_str());
    if (list->m_difficulty == -1) {
        diffSprite = CCSprite::createWithSpriteFrameName("difficulty_00_btn_001.png");
    }
    if (diffSprite) {
        diffSprite->setScale(0.55f);
        diffSprite->setPosition(ccp(15, 15.f));
        diffSprite->setID("diff-icon");
        row->addChild(diffSprite);
    }

    return row;
}

gd::string DictSortListPopup::keyForObject(cocos2d::CCObject* obj, int index) const {
    // GD stores favorite lists with the listID converted to string as key.
    // We use m_listID to ensure the key is always correct.
    auto list = static_cast<GJLevelList*>(obj);
    return fmt::format("{}", list->m_listID);
}

DictSortListPopup* DictSortListPopup::create(cocos2d::CCDictionary* dict) {
    if (!dict) return nullptr;

    auto ret = new DictSortListPopup();
    if (ret->init(dict)) {
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
