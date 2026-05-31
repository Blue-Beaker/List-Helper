#include "../main.hpp"
#include "../utils.hpp"
#include <Geode/modify/LevelBrowserLayer.hpp>

class MyPopup : public geode::Popup {
protected:
    bool init(cocos2d::CCArray * lists) {
        if (!Popup::init(240.f, 160.f))
            return false;

        // convenience function provided by Popup
        // for adding/setting a title to the popup
        this->setTitle("Sort Lists");


        if (lists)
        {
            for (auto i = 0; i < lists->count(); i++)
            {
                auto list = static_cast<GJLevelList *>(lists->objectAtIndex(i));
                
            }
        }

        // auto label = CCLabelBMFont::create(value.c_str(), "bigFont.fnt");
        // m_mainLayer->addChildAtPosition(label, Anchor::Center);

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