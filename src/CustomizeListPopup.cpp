#include "CustomizeListPopup.hpp"

bool CustomizeListPopup::setup(gd::string current) {
    this->setTitle("Customized Levels List");

    // Input label
    auto label = CCLabelBMFont::create("Input level IDs seperated by any non-digit chars", "bigFont.fnt");
    label->setScale(0.6f);
    m_mainLayer->addChildAtPosition(label, Anchor::Center, ccp(0, 25));

    // Text input
    input = TextInput::create(200.f, "", "chatFont.fnt");
    input->setString(current);
    m_mainLayer->addChildAtPosition(input, Anchor::Center, ccp(0, -5));

    // Apply button
    auto okBtn = CCMenuItemSpriteExtra::create(
        ButtonSprite::create("OK"),
        this,
        menu_selector(CustomizeListPopup::onApply)
    );

    // Cancel button
    auto cancelBtn = CCMenuItemSpriteExtra::create(
        ButtonSprite::create("Cancel"),
        this,
        menu_selector(CustomizeListPopup::onCancel)
    );

    auto menu = CCMenu::create();
    menu->addChild(okBtn);
    menu->addChild(cancelBtn);

    menu->alignItemsHorizontallyWithPadding(20);
    m_mainLayer->addChildAtPosition(menu, Anchor::Center, ccp(0, -50));

    return true;
}

void CustomizeListPopup::onApply(CCObject*) {
    levelString=this->input->getString();
    this->onClose(nullptr);
}

void CustomizeListPopup::onCancel(CCObject*) {
    this->onClose(nullptr);
}

CustomizeListPopup* CustomizeListPopup::create(gd::string current) {
    auto ret = new CustomizeListPopup();
    if (ret->init(500.f, 300.f)) {
        ret->setup(current);
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}
