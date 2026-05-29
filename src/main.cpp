#include "main.hpp"
#include "CustomizeListPopup.hpp"

gd::string levelString = "";

/**
 * `$modify` lets you extend and modify GD's classes.
 * To hook a function in Geode, simply $modify the class
 * and write a new function definition with the signature of
 * the function you want to hook.
 *
 * Here we use the overloaded `$modify` macro to set our own class name,
 * so that we can use it for button callbacks.
 *
 * Notice the header being included, you *must* include the header for
 * the class you are modifying, or you will get a compile error.
 *
 * Another way you could do this is like this:
 *
 * struct MyMenuLayer : Modify<MyMenuLayer, MenuLayer> {};
 */
#include <Geode/modify/CreatorLayer.hpp>
class $modify(MyMenuLayer, CreatorLayer) {
	/**
	 * Typically classes in GD are initialized using the `init` function, (though not always!),
	 * so here we use it to add our own button to the bottom menu.
	 *
	 * Note that for all hooks, your signature has to *match exactly*,
	 * `void init()` would not place a hook!
	*/
	bool init() {
		/**
		 * We call the original init function so that the
		 * original class is properly initialized.
		 */
		if (!CreatorLayer::init()) {
			return false;
		}

		/**
		 * You can use methods from the `geode::log` namespace to log messages to the console,
		 * being useful for debugging and such. See this page for more info about logging:
		 * https://docs.geode-sdk.org/tutorials/logging
		*/
		log::debug("Hello from my MenuLayer::init hook! This layer has {} children.", this->getChildrenCount());

		/**
		 * See this page for more info about buttons
		 * https://docs.geode-sdk.org/tutorials/buttons
		*/

		auto sprite = CircleButtonSprite::createWithSpriteFrameName(
			"GJ_listAddBtn_001.png",
			1.0f,
			CircleBaseColor::Green,
			CircleBaseSize::SmallAlt
		);
		auto myButton = CCMenuItemSpriteExtra::create(
			sprite,
			this,
			/**
			 * Here we use the name we set earlier for our modify class.
			*/
			menu_selector(MyMenuLayer::onMyButton)
		);

		/**
		 * Here we access the `bottom-menu` node by its ID, and add our button to it.
		 * Node IDs are a Geode feature, see this page for more info about it:
		 * https://docs.geode-sdk.org/tutorials/nodetree
		*/
		auto menu = this->getChildByID("bottom-right-menu");
		menu->addChild(myButton);

		/**
		 * The `_spr` string literal operator just prefixes the string with
		 * your mod id followed by a slash. This is good practice for setting your own node ids.
		*/
		myButton->setID("my-button"_spr);

		/**
		 * We update the layout of the menu to ensure that our button is properly placed.
		 * This is yet another Geode feature, see this page for more info about it:
		 * https://docs.geode-sdk.org/tutorials/layouts
		*/
		menu->updateLayout();

		/**
		 * We return `true` to indicate that the class was properly initialized.
		 */
		return true;
	}

	/**
	 * This is the callback function for the button we created earlier.
	 * The signature for button callbacks must always be the same,
	 * return type `void` and taking a `CCObject*`.
	*/

	void onMyButton(CCObject*) {
		// FLAlertLayer::create("Geode", "Hello from my custom mod!", "OK")->show();
		// auto myLevelBrowser = LevelBrowserLayer::create(GJSearchObject::create(SearchType::SavedLevels));
		// // myLevelBrowser->setupLevelBrowser(CCArray::create());
		// myLevelBrowser->show();
		auto popup = CustomizeListPopup::create(levelString);
		popup->show();
	}
};

#include <Geode/modify/LevelCell.hpp>

class $modify(MyLevelCell, LevelCell) {
	struct Fields {
		std::list<gd::string> favListNames = {};
		std::list<gd::string> createdListNames = {};
		gd::string levelName = "";
	};

	void loadFromLevel(GJGameLevel* level) {
		
		LevelCell::loadFromLevel(level);
		m_fields->levelName = level->m_levelName;

		auto manager = GameLevelManager::get();
		auto manager1 = LocalLevelManager::get();

		m_fields->favListNames.clear();
		m_fields->createdListNames.clear();

        m_fields->favListNames = getListsForLevel(manager->getSavedLevelLists(0), level);

        m_fields->createdListNames = getListsForLevel(manager1->getCreatedLists(0), level);

        // Remove old node
		this->removeChildByID("inListsMenu"_spr);

		// If not contained in any lists, do not add the button
		if(m_fields->createdListNames.empty() && m_fields->favListNames.empty()){
			return;
		}
		
		// Add the button
		auto labelSprite = CCSprite::createWithSpriteFrameName("GJ_listAddBtn_001.png");
		labelSprite->setScale(0.45);

		auto listsCountLabel = CCLabelBMFont::create(
			fmt::format("{}/{}",m_fields->createdListNames.size(),m_fields->favListNames.size()).c_str(), "bigFont.fnt"
		);
		listsCountLabel->setID("listsCountLabel"_spr);
		listsCountLabel->setScale(0.8);
		listsCountLabel->setPosition(35,-5);
		listsCountLabel->setAnchorPoint(ccp(1,0));
		labelSprite->addChild(listsCountLabel);

		auto listsButton = CCMenuItemSpriteExtra::create(
			labelSprite,
			this,
			menu_selector(MyLevelCell::onClickedButton)
		);
		listsButton->setID("listsButton"_spr);
		listsButton->setPosition(0,0);

		auto menu = CCMenu::create();
		menu->setID("listsMenu"_spr);
		menu->addChild(listsButton);
		menu->alignItemsHorizontallyWithPadding(2.0f);
		menu->setPosition(340,70);

		auto main_layer = this->getChildByID("main-layer");
		main_layer->addChild(menu);
    }

    std::list<gd::string> getListsForLevel(cocos2d::CCArray * lists, GJGameLevel * level)
    {
 		std::list<gd::string> listNames = {};
        if (lists)
        {
            for (auto i = 0; i < lists->count(); i++)
            {
                auto list = static_cast<GJLevelList *>(lists->objectAtIndex(i));
                bool containsLevel = false;
                auto levelsArray = list->getListLevelsArray(CCArray::create());
                if (levelsArray)
                {
                    for (auto j = 0; j < levelsArray->count(); j++)
                    {
                        auto lvl = static_cast<GJGameLevel *>(levelsArray->objectAtIndex(j));
                        if (lvl->m_levelID == level->m_levelID)
                        {
                            containsLevel = true;
                            break;
                        }
                    }
                }
                if (containsLevel)
                {
                    listNames.push_back(list->m_listName);
                }
            }
        }
		return listNames;
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

    void namesToString(std::list<gd::string> & favListNames, std::string & namesStr)
    {
        for (auto it = favListNames.begin(); it != favListNames.end(); ++it)
        {
            if (it != favListNames.begin())
                namesStr += ", ";
            namesStr += *it;
        }
    }
};