#pragma once

#include "ReorderListPopup.hpp"

/// A popup that takes a CCDictionary (int→GJLevelList),
/// converts it to an array internally, allows reordering,
/// and applies the new order back to the dict on close.
class DictSortListPopup : public ReorderListPopup {
protected:
    /// The original dictionary (int listID → GJLevelList*)
    cocos2d::CCDictionary* m_dict;

    /// Keep the array alive (ReorderListPopup only stores a raw pointer)
    cocos2d::CCArray* m_retainedArray;

    /// Snapshot of string keys in the original order, used to detect changes
    /// and rebuild the dict on close.
    std::vector<gd::string> m_originalKeys;

    bool init(cocos2d::CCDictionary* dict);
    CCNode* setupRow(int index) override;
    std::string getTitle() const override;
    void onClose(cocos2d::CCObject* sender) override;

public:
    static DictSortListPopup* create(cocos2d::CCDictionary* dict);
};
