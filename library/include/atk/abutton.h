#ifndef ABUTTON_H
#include <atk/awidget.h>
#include <atk/acontrolhost.h>
#include <windows.h>
#include <set>


class AButton : public AControlHost
{
public:
    AButton(AWidget *parent);
    const std::string &text();
    void setText(const std::string &text);

    ASignal<> *clicked;
    ASignal<AButton*> *buttonClicked;
protected:
    bool wmCommand(WPARAM wParam, LPARAM lParam, int &ret);
};

class ACheckBox : public AButton
{
public:
    ACheckBox(AWidget *parent);
    void setChecked(bool checked);
    bool isChecked();

    ASignal<bool> *toggled;
    ASignal<ACheckBox*, bool> *buttonToggled;

protected:
    bool wmCommand(WPARAM wParam, LPARAM lParam, int &ret);
    bool checked_;
};

class ARadioButton : public ACheckBox
{
public:
    enum TextAlignment { TextOnRight, TextOnLeft };
    ARadioButton(AWidget *parent);
    void setAlignment(TextAlignment text);
};


class ARadioGroup : public AObject
{
public:
    ARadioGroup(AObject *owner);
    void append(ARadioButton* button);
    void remove(ARadioButton* button);
    void select(ARadioButton* button);

    ASignal<ARadioButton*> *selected;
private:
    void radio_buttonClicked(AButton* button);
    ASlot<AButton*> *radio_clickedSlot;
    ASlot<AObject*> *radio_deletedSlot;

    std::set<ARadioButton*> radioButtons;
};


class AGroupBox : public AButton
{
public:
    AGroupBox(AWidget *parent);
};

#endif // ABUTTON_H
