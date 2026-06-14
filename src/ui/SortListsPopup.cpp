#include "SortListsPopup.hpp"
#include "../utils.hpp"

std::string SortListsPopup::getTitle() const {
    return "Sort Lists";
}

CCNode* SortListsPopup::setupRow(int index) {
    auto list = static_cast<GJLevelList*>(m_lists->objectAtIndex(index));
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

void SortListsPopup::onClose(CCObject* sender) {
    Popup::onClose(sender);
    // Refresh the LevelBrowserLayer to reflect reordered lists
    if (auto browserLayer = CCScene::get()->getChildByType<LevelBrowserLayer>(0)) {
        browserLayer->onRefresh(sender);
    }
}

SortListsPopup* SortListsPopup::create(cocos2d::CCArray* lists) {
    auto ret = new SortListsPopup();
    if (ret->init(lists)) {
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
}
