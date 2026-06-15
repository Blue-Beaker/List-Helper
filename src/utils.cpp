#include "main.hpp"
#include "utils.hpp"

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

void namesToString(std::list<gd::string> & favListNames, std::string & namesStr)
{
	for (auto it = favListNames.begin(); it != favListNames.end(); ++it)
	{
		if (it != favListNames.begin())
			namesStr += ", ";
		namesStr += *it;
	}
}

// --- Row UI helpers ---

void addLevelNameLabel(cocos2d::CCNode* row, GJGameLevel* level) {
    auto label = CCLabelBMFont::create(level->m_levelName.c_str(), "bigFont.fnt");
    label->setScale(0.35f);
    label->setAnchorPoint(ccp(0, 0.5f));
    label->setPosition(ccp(28, 15.f));
    label->setID("name-label");
    row->addChild(label);
}

void addListNameLabel(cocos2d::CCNode* row, GJLevelList* list) {
    auto label = CCLabelBMFont::create(list->m_listName.c_str(), "bigFont.fnt");
    label->setScale(0.35f);
    label->setAnchorPoint(ccp(0, 0.5f));
    label->setPosition(ccp(28, 15.f));
    label->setID("name-label");
    row->addChild(label);
}

void addLevelDifficultyIcon(cocos2d::CCNode* row, GJGameLevel* level) {
    int diff;
    if (level->m_demon) {
        diff = GJGameLevel::demonIconForDifficulty(DemonDifficultyType(level->m_demonDifficulty));
    } else {
        diff = level->getAverageDifficulty();
    }
    auto frame = GJDifficultySprite::getDifficultyFrame(diff, GJDifficultyName::Short);
    auto sprite = CCSprite::createWithSpriteFrameName(frame.c_str());
    if (sprite) {
        sprite->setScale(0.55f);
        sprite->setPosition(ccp(15, 15.f));
        sprite->setID("diff-icon");
        row->addChild(sprite);
    }
}

void addListDifficultyIcon(cocos2d::CCNode* row, GJLevelList* list) {
	const char *frame;
    if (list->m_difficulty == -1) {
		frame = "difficulty_00_btn_001.png";
	}else{
    	frame = GJLevelList::frameForListDifficulty(list->m_difficulty, DifficultyIconType::ShortText).c_str();
	}
    auto sprite = CCSprite::createWithSpriteFrameName(frame);

    if (sprite) {
        sprite->setScale(0.55f);
        sprite->setPosition(ccp(15, 15.f));
        sprite->setID("diff-icon");
        row->addChild(sprite);
    }
}

cocos2d::CCNode* createLevelRow(GJGameLevel* level) {
    if (!level) return nullptr;
    auto row = CCNode::create();
    row->setContentSize({ 250.f, 30.f });
    addLevelNameLabel(row, level);
    addLevelDifficultyIcon(row, level);
    return row;
}

cocos2d::CCNode* createListRow(GJLevelList* list) {
    if (!list) return nullptr;
    auto row = CCNode::create();
    row->setContentSize({ 250.f, 30.f });
    addListNameLabel(row, list);
    addListDifficultyIcon(row, list);
    return row;
}