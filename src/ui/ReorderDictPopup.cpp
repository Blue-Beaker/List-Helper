#include "ReorderDictPopup.hpp"

bool ReorderDictPopup::init(cocos2d::CCDictionary* dict, const std::string& title) {
    m_dict = dict;

    // Use allKeys() which returns CCArray of CCString*, then look up each one.
    auto keys = dict->allKeys();
    auto lists = CCArray::createWithCapacity(keys->count());

    for (auto i = 0; i < keys->count(); i++) {
        auto keyStr = static_cast<CCString*>(keys->objectAtIndex(i));
        auto obj = dict->objectForKey(keyStr->getCString());
        if (obj) {
            lists->addObject(obj);
            m_originalKeys.push_back(keyStr->getCString());
        }
    }

    // Retain the array to keep it alive — ReorderListPopup only stores a raw pointer
    m_retainedArray = lists;
    m_retainedArray->retain();

    return ReorderListPopup::init(lists, title);
}

gd::string ReorderDictPopup::keyForObject(cocos2d::CCObject* obj, int index) const {
    // Default: use the original string key
    if (index >= 0 && index < static_cast<int>(m_originalKeys.size())) {
        return m_originalKeys[index];
    }
    return "";
}

void ReorderDictPopup::onClose(CCObject* sender) {
    // Check if the order actually changed by comparing pointer identity
    // at each position between the original keys and current m_lists order
    bool dirty = false;
    if (m_originalKeys.size() == m_lists->count()) {
        for (auto i = 0; i < static_cast<int>(m_originalKeys.size()); i++) {
            auto origObj = m_dict->objectForKey(m_originalKeys[i]);
            auto currentObj = m_lists->objectAtIndex(i);
            if (origObj != currentObj) {
                dirty = true;
                break;
            }
        }
    } else {
        dirty = true;
    }

    if (dirty) {
        // Retain all objects so they survive removeAllObjects
        std::vector<CCObject*> retained;
        for (auto i = 0; i < m_lists->count(); i++) {
            auto obj = m_lists->objectAtIndex(i);
            obj->retain();
            retained.push_back(obj);
        }

        m_dict->removeAllObjects();

        // Re-insert in the new order
        for (auto i = 0; i < m_lists->count(); i++) {
            auto obj = m_lists->objectAtIndex(i);
            m_dict->setObject(obj, keyForObject(obj, i));
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

    onAfterClose(sender);
}

void ReorderDictPopup::onAfterClose(cocos2d::CCObject* sender) {
    // Default: no-op. Subclasses can override.
}
