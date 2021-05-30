#include <windows.h>
#include <atk/aapplication.h>
#include <atk/abutton.h>
#include <atk/alabel.h>
#include <atk/awindow.h>

class ExampleWindow : public AWindow {
public:
    ExampleWindow(AWidget *owner);
private:
    virtual bool createEvent() override;
    virtual void paintEvent() override;
};

ExampleWindow::ExampleWindow(AWidget *owner) : AWindow(owner)
{
    setObjectName("ExampleWindow");
    resize({640, 480});
    move({320, 240});
    setStyle(WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_SYSMENU);
}

bool ExampleWindow::createEvent()
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
    groupBox->resize({11*12, 9*3*5+9});

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

    auto label = new ALabel(this);
    label->move({11*1, 26*9});
    label->resize({11*10, 9*3});
    label->setText("Label");

    return true;
}

void ExampleWindow::paintEvent()
{
    APainter p(handle());
    AColor bg(GetSysColor(COLOR_BTNFACE));

    ABrush b(bg);
    p.setBrush(b);
    p.fillRect(ARect(0,0,size().width(),size().height()));
    AFont f("Segoe UI", 16, 400);
    p.setBgColor(bg);
    p.setFgColor(AColor(0x5a, 0x5a, 0x5a));
    p.setFont(f);
    p.drawText(0, 0, "This text is drawn with the painting api.");
}



int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow)
{
    AApplication app(hInstance, nCmdShow);
    ExampleWindow window(nullptr);
    ASlot<> terminateMe(nullptr, [&]() {
        app.quit();
    });
    window.destroyed->connect(&terminateMe);
    return app.run();
}
