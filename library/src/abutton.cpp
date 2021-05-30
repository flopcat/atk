#include "atk/abutton.h"

AButton::AButton(AWidget *owner) : AControlHost(owner)
{
    setObjectName("Button Object");
    clicked = new ASignal<>(this);
    buttonClicked = new ASignal<AButton*>(this);

    setChildClass("BUTTON");    
    setChildText("Button");
    //setMessageFunction(WM_COMMAND, &AButton::wmCommand);
}

const std::string &AButton::text()
{
    return childText();
}

void AButton::setText(const std::string &text)
{
    setChildText(text);
}

bool AButton::wmCommand(WPARAM wParam, LPARAM lParam, int &ret)
{
    if (HIWORD(wParam) != BN_CLICKED)
        return false;
    clicked->send();
    buttonClicked->send(this);
    return true;
}


ACheckBox::ACheckBox(AWidget *owner) : AButton(owner)
{
    setObjectName("CheckBox Object");
    toggled = new ASignal<bool>(this);
    buttonToggled = new ASignal<ACheckBox*,bool>(this);
    checked_ = false;

    setChildClass("BUTTON");    
    setChildStyle(BS_AUTOCHECKBOX);
    setChildText("Button");
    setMessageFunction(WM_COMMAND, &ACheckBox::wmCommand);
}

void ACheckBox::setChecked(bool checked)
{
    checked_ = checked;
    SendMessage(childHandle(), BM_SETCHECK, checked ? BST_CHECKED : BST_UNCHECKED, 0);
}

bool ACheckBox::isChecked()
{
    return checked_;
}

bool ACheckBox::wmCommand(WPARAM wParam, LPARAM lParam, int &ret)
{
    if (HIWORD(wParam) != BN_CLICKED)
        return false;
    checked_ = SendMessage(childHandle(), BM_GETCHECK, 0, 0) == BST_CHECKED;
    clicked->send();
    toggled->send(checked_);
    buttonClicked->send(this);
    buttonToggled->send(this, checked_);
    return true;
}

ARadioButton::ARadioButton(AWidget *parent) : ACheckBox(parent)
{
    setAlignment(TextOnRight);
}

void ARadioButton::setAlignment(TextAlignment alignment)
{
    setChildStyle(BS_AUTORADIOBUTTON | (alignment == TextOnLeft ? BS_LEFTTEXT : 0));
}

ARadioGroup::ARadioGroup(AObject *owner) : AObject(owner)
{
    radio_clickedSlot = new ASlot<AButton*>(this, &radio_buttonClicked);
    radio_deletedSlot = new ASlot<AObject*>(this, [this](AObject* obj) { remove((ARadioButton*)obj); });
    selected = new ASignal<ARadioButton*>(this);
}

void ARadioGroup::append(ARadioButton* button)
{
    if (radioButtons.count(button) < 1) {
        button->aboutToDeleteObject->connect(radio_deletedSlot);
        button->buttonClicked->connect(radio_clickedSlot);
        radioButtons.insert(button);
    }    
}

void ARadioGroup::remove(ARadioButton* button)
{
    button->aboutToDeleteObject->disconnect(radio_deletedSlot);
    button->buttonClicked->disconnect(radio_clickedSlot);
}

void ARadioGroup::select(ARadioButton *button)
{
    if (radioButtons.count(button) < 1)
        return;
    button->setChecked(true);
    radio_buttonClicked(button);
}

void ARadioGroup::radio_buttonClicked(AButton* button)
{
    ARadioButton *radio = (ARadioButton*)button;
    for (auto i = radioButtons.begin(); i != radioButtons.end(); i++) {
        if ((*i) != radio && (*i)->isChecked())
            (*i)->setChecked(false);
    }
    selected->send(radio);
}

AGroupBox::AGroupBox(AWidget *parent)
    : AButton(parent)
{
    setChildText("Group");
    setChildStyle(BS_GROUPBOX);
}
