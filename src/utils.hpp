
std::list<gd::string> getListsForLevel(cocos2d::CCArray *lists, GJGameLevel *level);

void namesToString(std::list<gd::string> &favListNames, std::string &namesStr);

// --- Row UI helpers for sort popups ---

/// Add a level name label to a row node. The label is positioned at (28, 15) with scale 0.35.
void addLevelNameLabel(cocos2d::CCNode* row, GJGameLevel* level);

/// Add a list name label to a row node. Same positioning as level name.
void addListNameLabel(cocos2d::CCNode* row, GJLevelList* list);

/// Add a difficulty icon for a level to a row node. Positioned at (15, 15) with scale 0.55.
void addLevelDifficultyIcon(cocos2d::CCNode* row, GJGameLevel* level);

/// Add a difficulty icon for a list to a row node. Positioned at (15, 15) with scale 0.55.
void addListDifficultyIcon(cocos2d::CCNode* row, GJLevelList* list);

/// Create a full level row with name label and difficulty icon. Content size is {250.f, 30.f}.
/// Returns nullptr if level is null.
cocos2d::CCNode* createLevelRow(GJGameLevel* level);

/// Create a full list row with name label and difficulty icon. Content size is {250.f, 30.f}.
/// Returns nullptr if list is null.
cocos2d::CCNode* createListRow(GJLevelList* list);