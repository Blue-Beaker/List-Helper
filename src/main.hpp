#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

extern gd::string levelString;

void getContainedListsForLevel(cocos2d::CCArray *lists, GJGameLevel *level, int &matched_count);

void getListsForLevel(cocos2d::CCArray *lists, GJGameLevel *level, int &matched_count);

void getListsForLevel(cocos2d::CCArray *lists, GJGameLevel *level);

void namesToString(std::list<gd::string> &favListNames, std::string &namesStr);
