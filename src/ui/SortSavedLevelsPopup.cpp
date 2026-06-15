#include "SortSavedLevelsPopup.hpp"

bool SortSavedLevelsPopup::init(cocos2d::CCArray* levels, const std::string& title) {
    if (!Popup::init(280.f, 290.f))
        return false;

    this->setTitle(title);
    m_levels = levels;
    m_levels->retain();

    // Build initial order: 0, 1, 2, ...
    m_order.resize(levels->count());
    for (int i = 0; i < levels->count(); i++) {
        m_order[i] = i;
    }

    m_scrollLayer = geode::ScrollLayer::create({ 250.f, 200.f });
    m_scrollLayer->m_contentLayer->setLayout(
        ScrollLayer::createDefaultListLayout(0.f)
    );

    rebuildList();

    m_scrollLayer->m_contentLayer->updateLayout();
    m_scrollLayer->setPosition(ccp(15, 45));
    m_mainLayer->addChild(m_scrollLayer);

    m_scrollLayer->scrollToTop();

    return true;
}

void SortSavedLevelsPopup::rebuildList() {
    m_scrollLayer->m_contentLayer->removeAllChildren();
    m_scrollLayer->m_contentLayer->setLayout(
        ScrollLayer::createDefaultListLayout(0.f)
    );

    for (auto i = 0; i < m_order.size(); i++)
    {
        auto row = setupRow(i);
        if (!row) continue;
        row->setID(fmt::format("list-row-{}", i));

        auto btnMenu = setupButtonMenu(i);
        if (btnMenu) {
            row->addChild(btnMenu);
        }

        // Highlight picked row
        if (m_pickedIndex == i) {
            auto hl = geode::NineSlice::create("GJ_square07.png");
            hl->setContentSize({ 250.f, 30.f });
            hl->setPosition(ccp(0, 0));
            hl->setAnchorPoint(ccp(0, 0));
            hl->setID("pick-highlight");
            hl->setZOrder(-1);
            row->addChild(hl);
        }

        m_scrollLayer->m_contentLayer->addChild(row);
    }

    m_scrollLayer->m_contentLayer->updateLayout();
}

CCNode* SortSavedLevelsPopup::setupRow(int displayIndex) {
    int actualIdx = m_order[displayIndex];
    if (actualIdx < 0 || actualIdx >= m_levels->count()) return nullptr;
    auto level = static_cast<GJGameLevel*>(m_levels->objectAtIndex(actualIdx));
    if (!level) return nullptr;

    auto row = CCNode::create();
    row->setContentSize({ 250.f, 30.f });

    // Level name label
    auto nameLabel = CCLabelBMFont::create(
        level->m_levelName.c_str(), "bigFont.fnt"
    );
    nameLabel->setScale(0.35f);
    nameLabel->setAnchorPoint(ccp(0, 0.5f));
    nameLabel->setPosition(ccp(28, 15.f));
    nameLabel->setID("name-label");
    row->addChild(nameLabel);

    // Difficulty icon
    int level_diff;
    if(level->m_demon){
        level_diff=GJGameLevel::demonIconForDifficulty(DemonDifficultyType(level->m_demonDifficulty));
    }else{
        level_diff=level->getAverageDifficulty();
    }
    auto diff = GJDifficultySprite::getDifficultyFrame(level_diff, GJDifficultyName::Short);
    auto diffSprite = CCSprite::createWithSpriteFrameName(diff.c_str());
    if (diffSprite) {
        diffSprite->setScale(0.55f);
        diffSprite->setPosition(ccp(15, 15.f));
        diffSprite->setID("diff-icon");
        row->addChild(diffSprite);
    }

    return row;
}

CCMenu* SortSavedLevelsPopup::setupButtonMenu(int displayIndex) {
    // Up button
    auto upSprite = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
    upSprite->setScale(0.55f);
    upSprite->setFlipX(true);
    upSprite->setRotation(-90.f);
    auto upBtn = CCMenuItemSpriteExtra::create(
        upSprite, this, menu_selector(SortSavedLevelsPopup::onMoveUp)
    );
    upBtn->setTag(displayIndex);
    upBtn->setID(fmt::format("up-btn-{}", displayIndex));

    // Down button
    auto downSprite = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
    downSprite->setScale(0.55f);
    downSprite->setRotation(-90.f);
    auto downBtn = CCMenuItemSpriteExtra::create(
        downSprite, this, menu_selector(SortSavedLevelsPopup::onMoveDown)
    );
    downBtn->setTag(displayIndex);
    downBtn->setID(fmt::format("down-btn-{}", displayIndex));

    // Pick / Place button
    CCMenuItemSpriteExtra* actionBtn;
    if (m_pickedIndex < 0) {
        auto pickSprite = CircleButtonSprite::createWithSpriteFrameName(
            "edit_flipYBtn_001.png", 1.0f, CircleBaseColor::Green, CircleBaseSize::Tiny
        );
        actionBtn = CCMenuItemSpriteExtra::create(
            pickSprite, this, menu_selector(SortSavedLevelsPopup::onPick)
        );
        actionBtn->setID(fmt::format("pick-btn-{}", displayIndex));
    } else {
        auto placeSprite = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
        placeSprite->setScale(0.55f);
        actionBtn = CCMenuItemSpriteExtra::create(
            placeSprite, this, menu_selector(SortSavedLevelsPopup::onPlace)
        );
        actionBtn->setID(fmt::format("place-btn-{}", displayIndex));
    }
    actionBtn->setTag(displayIndex);

    auto btnMenu = CCMenu::create();
    btnMenu->setID(fmt::format("btn-menu-{}", displayIndex));
    btnMenu->addChild(upBtn);
    btnMenu->addChild(downBtn);
    btnMenu->addChild(actionBtn);
    btnMenu->setPosition(ccp(220, 15.f));
    btnMenu->alignItemsHorizontallyWithPadding(2.f);

    return btnMenu;
}

void SortSavedLevelsPopup::onMoveUp(CCObject* sender) {
    auto btn = static_cast<CCMenuItemSpriteExtra*>(sender);
    int idx = btn->getTag();
    if (idx <= 0 || idx >= m_order.size()) return;

    std::swap(m_order[idx], m_order[idx - 1]);
    if (m_pickedIndex == idx) m_pickedIndex = idx - 1;
    else if (m_pickedIndex == idx - 1) m_pickedIndex = idx;
    rebuildList();
}

void SortSavedLevelsPopup::onMoveDown(CCObject* sender) {
    auto btn = static_cast<CCMenuItemSpriteExtra*>(sender);
    int idx = btn->getTag();
    if (idx < 0 || idx >= m_order.size() - 1) return;

    std::swap(m_order[idx], m_order[idx + 1]);
    if (m_pickedIndex == idx) m_pickedIndex = idx + 1;
    else if (m_pickedIndex == idx + 1) m_pickedIndex = idx;
    rebuildList();
}

void SortSavedLevelsPopup::onPick(CCObject* sender) {
    auto btn = static_cast<CCMenuItemSpriteExtra*>(sender);
    int tag = btn->getTag();
    if (tag < 0 || tag >= m_order.size()) return;
    m_pickedIndex = tag;
    rebuildList();
}

void SortSavedLevelsPopup::onPlace(CCObject* sender) {
    auto btn = static_cast<CCMenuItemSpriteExtra*>(sender);
    int targetIndex = btn->getTag();
    if (targetIndex < 0 || targetIndex >= m_order.size()) return;

    if (m_pickedIndex == targetIndex) {
        m_pickedIndex = -1;
        rebuildList();
        return;
    }
    if (m_pickedIndex < 0) return;

    int pickedVal = m_order[m_pickedIndex];
    m_order.erase(m_order.begin() + m_pickedIndex);
    m_order.insert(m_order.begin() + targetIndex, pickedVal);

    m_pickedIndex = -1;
    rebuildList();
}

void SortSavedLevelsPopup::onClose(CCObject* sender) {
    // Write the new order back to the levels via m_levelIndex.
    // GD sorts saved levels by m_levelIndex in descending order,
    // so we use reverse index.
    for (int i = 0; i < m_order.size(); i++) {
        int actualIdx = m_order[i];
        if (actualIdx < 0 || actualIdx >= m_levels->count()) continue;
        auto level = static_cast<GJGameLevel*>(m_levels->objectAtIndex(actualIdx));
        if (level) {
            level->m_levelIndex = m_order.size() - 1 - i;
        }
    }
    GameLevelManager::get()->updateLevelOrders();

    Popup::onClose(sender);

    // Refresh the LevelBrowserLayer
    if (auto browserLayer = CCScene::get()->getChildByType<LevelBrowserLayer>(0)) {
        browserLayer->onRefresh(sender);
    }
}

SortSavedLevelsPopup::~SortSavedLevelsPopup() {
    if (m_levels) {
        m_levels->release();
        m_levels = nullptr;
    }
}

SortSavedLevelsPopup* SortSavedLevelsPopup::create(cocos2d::CCArray* levels, const std::string& title) {
    auto ret = new SortSavedLevelsPopup();
    if (ret->init(levels, title)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}
