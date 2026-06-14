#include "MyPopup.hpp"
#include "../utils.hpp"
#include <Geode/ui/ScrollLayer.hpp>

bool MyPopup::init(cocos2d::CCArray* lists) {
    if (!Popup::init(280.f, 290.f))
        return false;

    this->setTitle("Sort Lists");
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

void MyPopup::rebuildList() {
    m_scrollLayer->m_contentLayer->removeAllChildren();
    m_scrollLayer->m_contentLayer->setLayout(
        ScrollLayer::createDefaultListLayout(0.f)
    );

    for (auto i = 0; i < m_lists->count(); i++)
    {
        auto list = static_cast<GJLevelList*>(m_lists->objectAtIndex(i));
        auto row = CCNode::create();
        row->setContentSize({ 250.f, 30.f });
        row->setID(fmt::format("list-row-{}", i));

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
            // Fallback: use the NA difficulty frame directly
            diffSprite = CCSprite::createWithSpriteFrameName("difficulty_00_btn_001.png");
        }
        if (diffSprite) {
            diffSprite->setScale(0.55f);
            diffSprite->setPosition(ccp(15, 15.f));
            diffSprite->setID("diff-icon");
            row->addChild(diffSprite);
        }

        // Up button
        auto upSprite = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
        upSprite->setScale(0.55f);
        upSprite->setFlipX(true);
        upSprite->setRotation(-90.f);
        auto upBtn = CCMenuItemSpriteExtra::create(
            upSprite, this, menu_selector(MyPopup::onMoveUp)
        );
        upBtn->setTag(i);
        upBtn->setID(fmt::format("up-btn-{}", i));

        // Down button
        auto downSprite = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
        downSprite->setScale(0.55f);
        downSprite->setRotation(-90.f);
        auto downBtn = CCMenuItemSpriteExtra::create(
            downSprite, this, menu_selector(MyPopup::onMoveDown)
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
                pickSprite, this, menu_selector(MyPopup::onPick)
            );
            actionBtn->setID(fmt::format("pick-btn-{}", i));
        } else {
            // Place mode: place button
            auto placeSprite = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
            placeSprite->setScale(0.55f);
            actionBtn = CCMenuItemSpriteExtra::create(
                placeSprite, this, menu_selector(MyPopup::onPlace)
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

void MyPopup::onMoveUp(CCObject* sender) {
    auto btn = static_cast<CCMenuItemSpriteExtra*>(sender);
    int idx = btn->getTag();
    if (idx <= 0) return;

    m_lists->exchangeObjectAtIndex(idx, idx - 1);
    if (m_pickedIndex == idx) m_pickedIndex = idx - 1;
    else if (m_pickedIndex == idx - 1) m_pickedIndex = idx;
    rebuildList();
}

void MyPopup::onMoveDown(CCObject* sender) {
    auto btn = static_cast<CCMenuItemSpriteExtra*>(sender);
    int idx = btn->getTag();
    if (idx >= m_lists->count() - 1) return;

    m_lists->exchangeObjectAtIndex(idx, idx + 1);
    if (m_pickedIndex == idx) m_pickedIndex = idx + 1;
    else if (m_pickedIndex == idx + 1) m_pickedIndex = idx;
    rebuildList();
}

void MyPopup::onPick(CCObject* sender) {
    auto btn = static_cast<CCMenuItemSpriteExtra*>(sender);
    m_pickedIndex = btn->getTag();
    rebuildList();
}

void MyPopup::onPlace(CCObject* sender) {
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
    auto pickedList = static_cast<GJLevelList*>(m_lists->objectAtIndex(m_pickedIndex));
    pickedList->retain();
    m_lists->removeObjectAtIndex(m_pickedIndex);
    m_lists->insertObject(pickedList, targetIndex);
    pickedList->release();

    m_pickedIndex = -1;
    rebuildList();
}

void MyPopup::onClose(CCObject* sender) {
    Popup::onClose(sender);
    // Refresh the LevelBrowserLayer to reflect reordered lists
    if (auto browserLayer = CCScene::get()->getChildByType<LevelBrowserLayer>(0)) {
        browserLayer->onRefresh(sender);
    }
}

MyPopup* MyPopup::create(cocos2d::CCArray* lists) {
    auto ret = new MyPopup();
    if (ret->init(lists)) {
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
}
