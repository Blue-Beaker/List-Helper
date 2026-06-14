#pragma once

#include "ReorderListPopup.hpp"

/// A popup that takes a CCDictionary (string key → CCObject),
/// converts it to an array internally, allows reordering,
/// and applies the new order back to the dict on close.
///
/// Subclasses must implement setupRow().
class ReorderDictPopup : public ReorderListPopup {
protected:
    /// The original dictionary
    cocos2d::CCDictionary* m_dict;

    /// Keep the array alive (ReorderListPopup only stores a raw pointer)
    cocos2d::CCArray* m_retainedArray;

    /// Snapshot of string keys in the original order, used to detect changes
    /// and rebuild the dict on close.
    std::vector<gd::string> m_originalKeys;

    bool init(cocos2d::CCDictionary* dict, const std::string& title);
    void onClose(cocos2d::CCObject* sender) override;

    /// Called after the dictionary has been updated (or skipped if not dirty)
    /// and the popup has closed. Subclasses can override to perform additional
    /// actions like refreshing the parent layer.
    virtual void onAfterClose(cocos2d::CCObject* sender);

    /// Subclasses can override to customize how the key is generated when
    /// writing back to the dictionary. Defaults to the original string key.
    virtual gd::string keyForObject(cocos2d::CCObject* obj, int index) const;
};
