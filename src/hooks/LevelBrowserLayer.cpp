#include "../main.hpp"
#include "../utils.hpp"
#include <Geode/modify/LevelBrowserLayer.hpp>
#include <Geode/ui/ScrollLayer.hpp>

class MyPopup : public geode::Popup {
protected:
    cocos2d::CCArray* m_lists;
    geode::ScrollLayer* m_scrollLayer;
    int m_pickedIndex = -1;

    bool init(cocos2d::CCArray * lists) {
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

        return true;
    }

    void rebuildList() {
        m_scrollLayer->m_contentLayer->removeAllChildren();
        m_scrollLayer->m_contentLayer->setLayout(
            ScrollLayer::createDefaultListLayout(0.f)
        );

        for (auto i = 0; i < m_lists->count(); i++)
        {
            auto list = static_cast<GJLevelList *>(m_lists->objectAtIndex(i));
            auto row = CCNode::create();
            row->setContentSize({ 250.f, 30.f });
            row->setID(fmt::format("list-row-{}", i));

            // List name label on the left
            auto nameLabel = CCLabelBMFont::create(
                list->m_listName.c_str(), "bigFont.fnt"
            );
            nameLabel->setScale(0.35f);
            nameLabel->setAnchorPoint(ccp(0, 0.5f));
            nameLabel->setPosition(ccp(5, 15.f));
            nameLabel->setID("name-label");
            row->addChild(nameLabel);

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
                auto pickSprite = CCSprite::createWithSpriteFrameName("GJ_plusBtn_001.png");
                pickSprite->setScale(0.55f);
                actionBtn = CCMenuItemSpriteExtra::create(
                    pickSprite, this, menu_selector(MyPopup::onPick)
                );
                actionBtn->setID(fmt::format("pick-btn-{}", i));
            } else {
                // Place mode: place button
                auto placeSprite = CCSprite::createWithSpriteFrameName("GJ_plusBtn_001.png");
                placeSprite->setScale(0.55f);
                placeSprite->setColor(ccc3(0x6a, 0xaa, 0x3c));
                actionBtn = CCMenuItemSpriteExtra::create(
                    placeSprite, this, menu_selector(MyPopup::onPlace)
                );
                actionBtn->setID(fmt::format("place-btn-{}", i));
            }
            actionBtn->setTag(i);

            auto btnMenu = CCMenu::create();
            btnMenu->setID(fmt::format("btn-menu-{}", i));
            btnMenu->addChild(upBtn);
            btnMenu->addChild(actionBtn);
            btnMenu->addChild(downBtn);
            btnMenu->setPosition(ccp(220, 15.f));
            btnMenu->alignItemsHorizontallyWithPadding(2.f);
            row->addChild(btnMenu);

            // Highlight picked row
            if (m_pickedIndex == i) {
                auto hl = CCSprite::createWithSpriteFrameName("GJ_select_001.png");
                hl->setScaleX(250.f / hl->getContentWidth());
                hl->setScaleY(30.f / hl->getContentHeight());
                hl->setPosition(ccp(125, 15));
                hl->setID("pick-highlight");
                hl->setZOrder(-1);
                row->addChild(hl);
            }

            m_scrollLayer->m_contentLayer->addChild(row);
        }

        m_scrollLayer->m_contentLayer->updateLayout();
    }

    void onMoveUp(CCObject* sender) {
        auto btn = static_cast<CCMenuItemSpriteExtra*>(sender);
        int idx = btn->getTag();
        if (idx <= 0) return;

        m_lists->exchangeObjectAtIndex(idx, idx - 1);
        if (m_pickedIndex == idx) m_pickedIndex = idx - 1;
        else if (m_pickedIndex == idx - 1) m_pickedIndex = idx;
        rebuildList();
    }

    void onMoveDown(CCObject* sender) {
        auto btn = static_cast<CCMenuItemSpriteExtra*>(sender);
        int idx = btn->getTag();
        if (idx >= m_lists->count() - 1) return;

        m_lists->exchangeObjectAtIndex(idx, idx + 1);
        if (m_pickedIndex == idx) m_pickedIndex = idx + 1;
        else if (m_pickedIndex == idx + 1) m_pickedIndex = idx;
        rebuildList();
    }

    void onPick(CCObject* sender) {
        auto btn = static_cast<CCMenuItemSpriteExtra*>(sender);
        m_pickedIndex = btn->getTag();
        rebuildList();
    }

    void onPlace(CCObject* sender) {
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

public:
    static MyPopup* create(cocos2d::CCArray * lists) {
        auto ret = new MyPopup();
        if (ret->init(lists)) {
            ret->autorelease();
            return ret;
        }

        delete ret;
        return nullptr;
    }
};

class $modify(MyLevelBrowserLayer, LevelBrowserLayer) {
    bool init(GJSearchObject* object) {
        if (!LevelBrowserLayer::init(object)) return false;
        
        // Limit the button to only saved and favorite lists
        if(this->m_searchObject==nullptr) return true;
        auto searchType = this->m_searchObject->m_searchType;
        if(searchType != SearchType::MyLists && searchType != SearchType::FavouriteLists) return true;
        
		auto reorderListsButton = CCMenuItemSpriteExtra::create(
			CircleButtonSprite::createWithSpriteFrameName(
                "edit_flipYBtn_001.png",
                1.0f,
                CircleBaseColor::Green,
                CircleBaseSize::Tiny
            ),
			this,
			menu_selector(MyLevelBrowserLayer::onClickedButton)
		);
		reorderListsButton->setID("reorderListsButton"_spr);
		reorderListsButton->setPosition(0,0);

		auto page_menu = this->getChildByID("page-menu");
        if(page_menu==nullptr) return true;
		page_menu->addChild(reorderListsButton);
        return true;
    }
    void onClickedButton(CCObject*){
        auto searchType = this->m_searchObject->m_searchType;
        if(searchType == SearchType::MyLists){
            MyPopup::create(LocalLevelManager::get()->m_localLists)->show();
        }else if (searchType == SearchType::FavouriteLists){
            // m_favoriteLists is a CCDictionary: key = listID (int), value = GJLevelList*
            auto dict = GameLevelManager::get()->m_favoriteLists;
            auto lists = CCArray::create();
            if (dict) {
                auto keys = dict->allKeys();
                for (auto i = 0; i < keys->count(); i++) {
                    auto key = static_cast<CCString*>(keys->objectAtIndex(i));
                    int listID = key->intValue();
                    auto list = static_cast<GJLevelList*>(dict->objectForKey(listID));
                    if (list) lists->addObject(list);
                }
            }
            MyPopup::create(lists)->show();
        }
    }
};