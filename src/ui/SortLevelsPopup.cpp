#include "SortLevelsPopup.hpp"
#include "../utils.hpp"

std::string SortLevelsPopup::getTitle() const {
    return "Sort Levels";
}

CCNode* SortLevelsPopup::setupRow(int index) {
    auto level = static_cast<GJGameLevel*>(m_lists->objectAtIndex(index));
    auto row = CCNode::create();
    row->setContentSize({ 250.f, 30.f });

    // List name label on the left
    auto nameLabel = CCLabelBMFont::create(
        level->m_levelName.c_str(), "bigFont.fnt"
    );
    nameLabel->setScale(0.35f);
    nameLabel->setAnchorPoint(ccp(0, 0.5f));
    nameLabel->setPosition(ccp(28, 15.f));
    nameLabel->setID("name-label");
    row->addChild(nameLabel);
    
    // Difficulty icon using the level's difficulty frame
    auto diff = GJDifficultySprite::getDifficultyFrame(int(level->m_difficulty),
    GJDifficultyName::Short);

    auto diffSprite = CCSprite::createWithSpriteFrameName(diff.c_str());
    // if (level->m_difficulty == GJDifficulty::NA) {
    //     diffSprite = CCSprite::createWithSpriteFrameName("difficulty_00_btn_001.png");
    // }
    if (diffSprite) {
        diffSprite->setScale(0.55f);
        diffSprite->setPosition(ccp(15, 15.f));
        diffSprite->setID("diff-icon");
        row->addChild(diffSprite);
    }

    return row;
}

void SortLevelsPopup::onClose(CCObject* sender) {
    Popup::onClose(sender);
    // Refresh the LevelBrowserLayer to reflect reordered levels
    if (auto browserLayer = CCScene::get()->getChildByType<LevelBrowserLayer>(0)) {
        browserLayer->onRefresh(sender);
    }
}

SortLevelsPopup* SortLevelsPopup::create(cocos2d::CCArray* levels) {
    auto ret = new SortLevelsPopup();
    if (ret->init(levels)) {
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
}
