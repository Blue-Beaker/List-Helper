#include "../main.hpp"
#include "../utils.hpp"
#include <Geode/modify/LevelCell.hpp>

class $modify(MyLevelCell, LevelCell) {
	struct Fields {
		std::list<gd::string> favListNames = {};
		std::list<gd::string> createdListNames = {};
		gd::string levelName = "";
	};

	void loadFromLevel(GJGameLevel* level) {
		
		LevelCell::loadFromLevel(level);

		if(level->m_levelID<=100){
			return;
		}
		// If the level is actually a list, return
		if (dynamic_cast<GJLevelList*>(level)) {
			return;
		}



		m_fields->levelName = level->m_levelName;

		auto manager = GameLevelManager::get();
		auto manager1 = LocalLevelManager::get();

		m_fields->favListNames.clear();
		m_fields->createdListNames.clear();

        m_fields->favListNames = getListsForLevel(manager->getSavedLevelLists(0), level);

        m_fields->createdListNames = getListsForLevel(manager1->getCreatedLists(0), level);

        // Remove old node
		this->removeChildByID("inListsMenu"_spr);

		// If not contained in any lists, hide
		if(m_fields->createdListNames.empty() && m_fields->favListNames.empty()){
			return;
		}
		// In list view, if only 1 list contained this(probably the currently viewing list), hide
		if(m_compactView && (m_fields->createdListNames.size() + m_fields->favListNames.size() <=1)){
			return;
		}
		
        auto button = CCNode::create();

		// Add the button
		auto labelSprite = CCSprite::createWithSpriteFrameName("GJ_listAddBtn_001.png");

        float maxLabelScale;
		if(this->m_compactView){
			labelSprite->setScale(0.3);
            maxLabelScale=0.3;
		}else{
			labelSprite->setScale(0.45);
            maxLabelScale=0.4;
		}


		auto listsCountLabel = CCLabelBMFont::create(
			fmt::format("{}/{}",m_fields->createdListNames.size(),m_fields->favListNames.size()).c_str(), "bigFont.fnt"
		);
		listsCountLabel->setID("listsCountLabel"_spr);
		listsCountLabel->setScale(std::min(maxLabelScale,50.0f/listsCountLabel->getContentWidth()));

        float height = std::max(labelSprite->getScaledContentHeight(),listsCountLabel->getScaledContentHeight());

        labelSprite->setPosition(ccp(0,height/2));
        labelSprite->setAnchorPoint(ccp(0,0.5));
		listsCountLabel->setPosition(labelSprite->getScaledContentWidth(),height/2);
		listsCountLabel->setAnchorPoint(ccp(0,0.5));

        button->addChild(labelSprite);
		button->addChild(listsCountLabel);

        button->setContentWidth(labelSprite->getScaledContentWidth()+listsCountLabel->getScaledContentWidth());
        button->setContentHeight(height);

        button->setAnchorPoint(ccp(0.5,0.5));
		// labelSprite->addChild(listsCountLabel);

		auto listsButton = CCMenuItemSpriteExtra::create(
			button,
			this,
			menu_selector(MyLevelCell::onClickedButton)
		);
		listsButton->setID("listsButton"_spr);
		listsButton->setPosition(0,0);

		auto menu = CCMenu::create();
		menu->setID("listsMenu"_spr);
		menu->addChild(listsButton);
		menu->alignItemsHorizontallyWithPadding(2.0f);
		
		if(this->m_compactView){
			// List
			menu->setPosition(260,8.5);
		}else{
			// Level Search, Saved levels...
			menu->setPosition(310,70);
		}

		auto main_layer = this->getChildByID("main-layer");
        if(main_layer==nullptr) return;
		main_layer->addChild(menu);
    }

    void onClickedButton(CCObject*){
		std::string namesStr;
		if(!m_fields->createdListNames.empty()){
			namesStr += "Created Lists:\n";
			namesToString(m_fields->createdListNames, namesStr);
		}
		if (!m_fields->createdListNames.empty() && !m_fields->favListNames.empty()){
			namesStr+="\n";
		}
		if(!m_fields->favListNames.empty()){
			namesStr += "Favorite Lists:\n";
			namesToString(m_fields->favListNames, namesStr);
		}

        FLAlertLayer::create(fmt::format("Lists containing {}", m_fields->levelName).c_str(), namesStr.c_str(), "OK")->show();
    }
};