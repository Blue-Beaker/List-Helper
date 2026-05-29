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