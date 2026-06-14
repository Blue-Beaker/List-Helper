#include "ReorderListPopup.hpp"

bool ReorderListPopup::init(cocos2d::CCArray* lists, const std::string& title) {
    if (!Popup::init(280.f, 290.f))
        return false;

    m_title = title;
    this->setTitle(m_title);
    m_lists = lists;

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

void ReorderListPopup::rebuildList() {
    m_scrollLayer->m_contentLayer->removeAllChildren();
    m_scrollLayer->m_contentLayer->setLayout(
        ScrollLayer::createDefaultListLayout(0.f)
    );

    for (auto i = 0; i < m_lists->count(); i++)
    {
        auto row = setupRow(i);
        row->setID(fmt::format("list-row-{}", i));

        // Up button
        auto upSprite = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
        upSprite->setScale(0.55f);
        upSprite->setFlipX(true);
        upSprite->setRotation(-90.f);
        auto upBtn = CCMenuItemSpriteExtra::create(
            upSprite, this, menu_selector(ReorderListPopup::onMoveUp)
        );
        upBtn->setTag(i);
        upBtn->setID(fmt::format("up-btn-{}", i));

        // Down button
        auto downSprite = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
        downSprite->setScale(0.55f);
        downSprite->setRotation(-90.f);
        auto downBtn = CCMenuItemSpriteExtra::create(
            downSprite, this, menu_selector(ReorderListPopup::onMoveDown)
        );
        downBtn->setTag(i);
        downBtn->setID(fmt::format("down-btn-{}", i));

        // Pick / Place button
        CCMenuItemSpriteExtra* actionBtn;
        if (m_pickedIndex < 0) {
            // Normal mode: pick button
            auto pickSprite = CircleButtonSprite::createWithSpriteFrameName(
                "edit_flipYBtn_001.png",
                1.0f,
                CircleBaseColor::Green,
                CircleBaseSize::Tiny
            );
            actionBtn = CCMenuItemSpriteExtra::create(
                pickSprite, this, menu_selector(ReorderListPopup::onPick)
            );
            actionBtn->setID(fmt::format("pick-btn-{}", i));
        } else {
            // Place mode: place button
            auto placeSprite = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
            placeSprite->setScale(0.55f);
            actionBtn = CCMenuItemSpriteExtra::create(
                placeSprite, this, menu_selector(ReorderListPopup::onPlace)
            );
            actionBtn->setID(fmt::format("place-btn-{}", i));
        }
        actionBtn->setTag(i);

        auto btnMenu = CCMenu::create();
        btnMenu->setID(fmt::format("btn-menu-{}", i));
        btnMenu->addChild(upBtn);
        btnMenu->addChild(downBtn);
        btnMenu->addChild(actionBtn);
        btnMenu->setPosition(ccp(220, 15.f));
        btnMenu->alignItemsHorizontallyWithPadding(2.f);
        row->addChild(btnMenu);

        // Highlight picked row
        if (m_pickedIndex == i) {
            auto hl = geode::NineSlice::create(
                "GJ_square07.png"
            );
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

void ReorderListPopup::onMoveUp(CCObject* sender) {
    auto btn = static_cast<CCMenuItemSpriteExtra*>(sender);
    int idx = btn->getTag();
    if (idx <= 0) return;

    m_lists->exchangeObjectAtIndex(idx, idx - 1);
    if (m_pickedIndex == idx) m_pickedIndex = idx - 1;
    else if (m_pickedIndex == idx - 1) m_pickedIndex = idx;
    rebuildList();
}

void ReorderListPopup::onMoveDown(CCObject* sender) {
    auto btn = static_cast<CCMenuItemSpriteExtra*>(sender);
    int idx = btn->getTag();
    if (idx >= m_lists->count() - 1) return;

    m_lists->exchangeObjectAtIndex(idx, idx + 1);
    if (m_pickedIndex == idx) m_pickedIndex = idx + 1;
    else if (m_pickedIndex == idx + 1) m_pickedIndex = idx;
    rebuildList();
}

void ReorderListPopup::onPick(CCObject* sender) {
    auto btn = static_cast<CCMenuItemSpriteExtra*>(sender);
    m_pickedIndex = btn->getTag();
    rebuildList();
}

void ReorderListPopup::onPlace(CCObject* sender) {
    auto btn = static_cast<CCMenuItemSpriteExtra*>(sender);
    int targetIndex = btn->getTag();

    // Clicking the same row cancels the pick
    if (m_pickedIndex == targetIndex) {
        m_pickedIndex = -1;
        rebuildList();
        return;
    }
    if (m_pickedIndex < 0) return;

    // Remove from picked position and insert at target
    auto pickedObj = m_lists->objectAtIndex(m_pickedIndex);
    pickedObj->retain();
    m_lists->removeObjectAtIndex(m_pickedIndex);
    m_lists->insertObject(pickedObj, targetIndex);
    pickedObj->release();

    m_pickedIndex = -1;
    rebuildList();
}


