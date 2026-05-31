#include "../main.hpp"
#include "../utils.hpp"
#include <Geode/modify/LevelBrowserLayer.hpp>
#include <Geode/ui/ScrollLayer.hpp>

class MyPopup : public geode::Popup {
protected:
    bool init(cocos2d::CCArray * lists) {
        if (!Popup::init(280.f, 260.f))
            return false;

        this->setTitle("Sort Lists");

        // Scrollable list showing all list names
        auto scrollLayer = geode::ScrollLayer::create({ 190.f, 180.f });
        scrollLayer->m_contentLayer->setLayout(
            ScrollLayer::createDefaultListLayout(0.f)
        );

        if (lists)
        {
            for (auto i = 0; i < lists->count(); i++)
            {
                auto list = static_cast<GJLevelList *>(lists->objectAtIndex(i));
                auto row = CCNode::create();
                row->setContentSize({ 190.f, 30.f });
                row->setID(fmt::format("list-row-{}", i));

                auto nameLabel = CCLabelBMFont::create(
                    list->m_listName.c_str(), "bigFont.fnt"
                );
                nameLabel->setScale(0.35f);
                nameLabel->setAnchorPoint(ccp(0, 0.5f));
                nameLabel->setPosition(ccp(5, 15.f));
                nameLabel->setID("name-label");
                row->addChild(nameLabel);

                scrollLayer->m_contentLayer->addChild(row);
            }
        }

        scrollLayer->m_contentLayer->updateLayout();
        scrollLayer->setPosition(ccp(10, 40));
        m_mainLayer->addChild(scrollLayer);

        // Up / Down buttons on the right side of the list (no functionality yet)
        auto ctrlMenu = CCMenu::create();
        ctrlMenu->setID("ctrl-menu"_spr);

        auto upSprite = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
        upSprite->setScale(0.7f);
        upSprite->setFlipX(true);
        upSprite->setRotation(-90.f);
        auto upBtn = CCMenuItemSpriteExtra::create(
            upSprite, nullptr, nullptr
        );
        upBtn->setID("move-up-btn"_spr);

        auto downSprite = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
        downSprite->setScale(0.7f);
        downSprite->setRotation(-90.f);
        auto downBtn = CCMenuItemSpriteExtra::create(
            downSprite, nullptr, nullptr
        );
        downBtn->setID("move-down-btn"_spr);

        ctrlMenu->addChild(upBtn);
        ctrlMenu->addChild(downBtn);
        ctrlMenu->setPosition(ccp(240, 130));
        ctrlMenu->alignItemsVerticallyWithPadding(15.f);
        m_mainLayer->addChild(ctrlMenu);

        return true;
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
            MyPopup::create(LocalLevelManager::get()->getCreatedLists(0))->show();
        }else if (searchType == SearchType::FavouriteLists){
            MyPopup::create(GameLevelManager::get()->getSavedLevelLists(0))->show();
        }
    }
};