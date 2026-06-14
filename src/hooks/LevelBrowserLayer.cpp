#include "../main.hpp"
#include "../utils.hpp"
#include "../ui/SortListsPopup.hpp"
#include "../ui/SortLevelsPopup.hpp"
#include "../ui/DictSortListPopup.hpp"
#include <Geode/modify/LevelBrowserLayer.hpp>

class $modify(MyLevelBrowserLayer, LevelBrowserLayer) {
    bool init(GJSearchObject* object) {
        if (!LevelBrowserLayer::init(object)) return false;
        
        if(this->m_searchObject==nullptr) return true;

        // Config
		if(!Mod::get()->getSettingValue<bool>("lists-sort")){
			return true;
		}

        // Limit the button to only saved and favorite lists
        auto searchType = this->m_searchObject->m_searchType;
        if(searchType != SearchType::MyLists
            && searchType != SearchType::FavouriteLists
            && searchType != SearchType::MyLevels
            && searchType != SearchType::SavedLevels) return true;
        
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
            SortListsPopup::create(LocalLevelManager::get()->m_localLists)->show();
        }else if (searchType == SearchType::FavouriteLists){
            if (auto popup = DictSortListPopup::create(GameLevelManager::get()->m_favoriteLists)) {
                popup->show();
            }
        }else if(searchType==SearchType::MyLevels){
            SortLevelsPopup::create(LocalLevelManager::get()->m_localLevels)->show();
        }else if(searchType==SearchType::SavedLevels){
        }
    }
};