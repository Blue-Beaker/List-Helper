#include "../main.hpp"
#include "../utils.hpp"
#include <Geode/modify/LevelBrowserLayer.hpp>

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
        
    }
};