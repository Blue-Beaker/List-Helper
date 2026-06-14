#include "DictSortListPopup.hpp"
#include "../utils.hpp"

bool DictSortListPopup::init(cocos2d::CCDictionary* dict) {
    m_dict = dict;

    // Use allKeys() which returns CCArray of CCString*, then look up each one.
    auto keys = dict->allKeys();
    auto lists = CCArray::createWithCapacity(keys->count());

    for (auto i = 0; i < keys->count(); i++) {
        auto keyStr = static_cast<CCString*>(keys->objectAtIndex(i));
        auto list = static_cast<GJLevelList*>(dict->objectForKey(keyStr->getCString()));
        if (list) {
            lists->addObject(list);
            m_originalKeys.push_back(keyStr->getCString());
        }
    }

    // Retain the array to keep it alive — ReorderListPopup only stores a raw pointer
    m_retainedArray = lists;
    m_retainedArray->retain();

    return ReorderListPopup::init(lists);
}

std::string DictSortListPopup::getTitle() const {
    return "Sort Favorite Lists";
}

CCNode* DictSortListPopup::setupRow(int index) {
    auto list = static_cast<GJLevelList*>(m_lists->objectAtIndex(index));
    if (!list) return CCNode::create();
    auto row = CCNode::create();
    row->setContentSize({ 250.f, 30.f });

    // List name label on the left
    auto nameLabel = CCLabelBMFont::create(
        list->m_listName.c_str(), "bigFont.fnt"
    );
    nameLabel->setScale(0.35f);
    nameLabel->setAnchorPoint(ccp(0, 0.5f));
    nameLabel->setPosition(ccp(28, 15.f));
    nameLabel->setID("name-label");
    row->addChild(nameLabel);

    // Difficulty icon using the list's difficulty frame
    auto diffFrame = GJLevelList::frameForListDifficulty(list->m_difficulty, DifficultyIconType::ShortText);
    auto diffSprite = CCSprite::createWithSpriteFrameName(diffFrame.c_str());
    if (list->m_difficulty == -1) {
        diffSprite = CCSprite::createWithSpriteFrameName("difficulty_00_btn_001.png");
    }
    if (diffSprite) {
        diffSprite->setScale(0.55f);
        diffSprite->setPosition(ccp(15, 15.f));
        diffSprite->setID("diff-icon");
        row->addChild(diffSprite);
    }

    return row;
}

void DictSortListPopup::onClose(CCObject* sender) {
    // Check if the order actually changed by comparing pointer identity
    // at each position between the original keys and current m_lists order
    bool dirty = false;
    if (m_originalKeys.size() == m_lists->count()) {
        for (auto i = 0; i < m_originalKeys.size(); i++) {
            auto origList = static_cast<GJLevelList*>(m_dict->objectForKey(m_originalKeys[i]));
            auto currentList = static_cast<GJLevelList*>(m_lists->objectAtIndex(i));
            if (origList != currentList) {
                dirty = true;
                break;
            }
        }
    } else {
        dirty = true;
    }

    if (dirty) {
        // Retain all lists so they survive removeAllObjects
        std::vector<CCObject*> retained;
        for (auto i = 0; i < m_lists->count(); i++) {
            auto obj = m_lists->objectAtIndex(i);
            obj->retain();
            retained.push_back(obj);
        }

        m_dict->removeAllObjects();

        // Re-insert in the new order using the list's m_listID converted to string.
        // CRITICAL: CCDictionary's key type is locked to string after first string-key insert.
        // Using setObject with int key would fail silently! We must use string keys.
        for (auto i = 0; i < m_lists->count(); i++) {
            auto list = static_cast<GJLevelList*>(m_lists->objectAtIndex(i));
            m_dict->setObject(list, fmt::format("{}", list->m_listID));
        }

        // Release retained objects
        for (auto obj : retained) {
            obj->release();
        }
    }

    // Release the array we retained in init
    m_retainedArray->release();
    m_retainedArray = nullptr;

    Popup::onClose(sender);

    // Refresh the LevelBrowserLayer to reflect reordered lists
    if (auto browserLayer = CCScene::get()->getChildByType<LevelBrowserLayer>(0)) {
        browserLayer->onRefresh(sender);
    }
}

DictSortListPopup* DictSortListPopup::create(cocos2d::CCDictionary* dict) {
    if (!dict) return nullptr;

    auto ret = new DictSortListPopup();
    if (ret->init(dict)) {
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
}
