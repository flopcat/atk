#include <aapplication.h>
#include <awindow.h>
#include "abutton.h"
#include <agraphics.h>
#include <iostream>
#include <utf8.h>
#include <windows.h>

AWindow::AWindow(AWidget *owner) : AWidget(owner)
{
    resize({640, 480});
    move({320, 240});
    setStyle(WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_SYSMENU);
}

bool AWindow::createEvent()
{
    std::cout << "Inside window createEvent" << std::endl;
    auto button = new AButton(this);
    button->move({11*1,9*1});
    button->resize({110,27});
    button->setText("Test");
    button->clicked->connect(new ASlot<>(this, []() {
        std::cout << "button clicked" << std::endl;
    }));

    auto check = new ACheckBox(this);
    check->move({11*1,9*5});
    check->resize({110,27});
    check->setText("Test");
    check->toggled->connect(new ASlot<bool>(this, [](bool checked) {
        std::cout << "button checked " << (checked ? "y" : "n") << std::endl;
    }));

    auto groupBox = new AGroupBox(this);
    groupBox->move({11*1,9*9});
    groupBox->resize({11*12,27*5+9});

    auto radioGroup = new ARadioGroup(this);
    auto radio = new ARadioButton(groupBox);
    radio->move({11*1,27*1});
    radio->resize({110,27});
    radio->setText("Radio1");
    radioGroup->append(radio);

    radio = new ARadioButton(groupBox);
    radio->move({11*1,27*2});
    radio->resize({110,27});
    radio->setText("Radio2");
    radioGroup->append(radio);

    radio = new ARadioButton(groupBox);
    radio->move({11*1,27*3});
    radio->resize({110,27});
    radio->setText("Radio3");
    radioGroup->append(radio);

    radio = new ARadioButton(groupBox);
    radio->move({11*1,27*4});
    radio->resize({110,27});
    radio->setText("Radio4");
    radioGroup->append(radio);

    radioGroup->selected->connect(new ASlot<ARadioButton*>(this, [](ARadioButton* radio) {
        std::cout << "button " << radio->text() << " selected" << std::endl;
    }));

    return true;
}

void AWindow::paintEvent()
{
    APainter p(handle());
    AColor bg(GetSysColor(COLOR_BTNFACE));

    ABrush b(bg);
    p.setBrush(b);
    p.fillRect(ARect(0,0,size().width(),size().height()));
    AFont f("MS Sans Serif", 16, 400);
    p.setBgColor(bg);
    p.setFgColor(AColor(0x5a, 0x5a, 0x5a));
    p.setFont(f);
    p.drawText(0, 0, "Hello Windows!");
}
