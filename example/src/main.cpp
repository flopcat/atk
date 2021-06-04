#include <windows.h>
#include <atk/aapplication.h>
#include <atk/abutton.h>
#include <atk/aeditor.h>
#include <atk/alabel.h>
#include <atk/awindow.h>

class ExampleWindow : public AWindow {
public:
    ExampleWindow(AWidget *owner);
private:
    // event callbacks
    virtual bool createEvent() override;
    virtual void paintEvent() override;

    // slot callbacks
    void check_checked(bool checked);
};

ExampleWindow::ExampleWindow(AWidget *owner) : AWindow(owner)
{
    // Set the object name, useful for debugging purposes.
    setObjectName("ExampleWindow");

    // Resize and move the window into a suitable location.
    resize({640, 480});
    move({320, 240});

    // Set a standard window style.  This is set in AWindow as well, so this
    // can be dropped if you are fine with the defaults.  Changing the style is
    // how you would create a fixed-size window, for example.
    setStyle(WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_SYSMENU);
}

// createEvent is called when processing the WM_CREATE message.  Return false
// when you don't want the created event to be sent.  The default return is
// true.
bool ExampleWindow::createEvent()
{
    // Column 1 - buttons and derivatives
    std::cout << "Inside window createEvent" << std::endl;
    auto button = new AButton(this);
    button->move({11*1,9*1});
    button->resize({110,27});
    button->setText("Test");
    button->clicked->connect(new ASlot<>(this, []() {
        // Print a message whenever the button is clicked
        std::cout << "button clicked" << std::endl;
    }));

    auto check = new ACheckBox(this);
    check->move({11*1,9*5});
    check->resize({110,27});
    check->setText("Test");
    check->toggled->connect(new ASlot<bool>(this, &ExampleWindow::check_checked));
    // ^ This code could be in a lambda like the rest of the examples like so:
    // new ASlot<bool>(this, [](bool checked) { ... }).  But to give an example
    // of this method we are doing it this way just this once.  In fact in
    // large code-bases you should probably use member functions.

    auto groupBox = new AGroupBox(this);
    groupBox->move({11*1,9*9});
    groupBox->resize({11*10, 9*3*5+9});

    // ARadioGroup takes care of ensuring that only one associated radio button
    // is selected at a time.
    auto radioGroup = new ARadioGroup(this);
    auto radio = new ARadioButton(groupBox);
    radio->move({11*1,27*1});
    radio->resize({11*8,27});
    radio->setText("Radio1");
    radio->setChecked(true);
    radioGroup->append(radio);

    radio = new ARadioButton(groupBox);
    radio->move({11*1,27*2});
    radio->resize({11*8,27});
    radio->setText("Radio2");
    radioGroup->append(radio);

    radio = new ARadioButton(groupBox);
    radio->move({11*1,27*3});
    radio->resize({11*8,27});
    radio->setText("Radio3");
    radioGroup->append(radio);

    radio = new ARadioButton(groupBox);
    radio->move({11*1,27*4});
    radio->resize({11*8,27});
    radio->setText("Radio4");
    radioGroup->append(radio);

    radioGroup->selected->connect(new ASlot<ARadioButton*>(this, [](ARadioButton* radio) {
        std::cout << "button " << radio->text() << " selected" << std::endl;
    }));

    // Column 2 - label and frames
    auto label = new ALabel(this);
    label->move({11*12, 9*1});
    label->resize({11*10, 9*2});
    label->setText("NoFrame");

    label = new ALabel(this);
    label->move({11*12, 9*4});
    label->resize({11*10, 9*2});
    label->setFrameEdge(ALabel::GrayFrame);
    label->setText("GrayFrame");

    label = new ALabel(this);
    label->move({11*12, 9*7});
    label->resize({11*10, 9*2});
    label->setFrameEdge(ALabel::WhiteFrame);
    label->setText("WhiteFrame");

    label = new ALabel(this);
    label->move({11*12, 9*10});
    label->resize({11*10, 9*2});
    label->setFrameEdge(ALabel::BlackFrame);
    label->setText("BlackFrame");

    label = new ALabel(this);
    label->move({11*12, 9*13});
    label->resize({11*10, 9*2});
    label->setFrameEdge(ALabel::SunkenFrame);
    label->setText("SunkenFrame");

    label = new ALabel(this);
    label->move({11*12, 9*16});
    label->resize({11*10, 9*2});
    label->setFrameEdge(ALabel::EtchedFrame);
    label->setText("EtchedFrame");

    // Column 3 - editors
    auto editor = new AEditor(this);
    editor->move({11*23, 9*1 });
    editor->resize({11*10, 9*2 + 1});
    editor->setText("Editor");
    editor->editorTextChanged->connect(new ASlot<AEditor*>(this, [](AEditor *editor) {
        std::cout << "editor changed to " << editor->text() << std::endl;
    }));

    editor = new AEditor(this);
    editor->move({11*23, 9*4 });
    editor->resize({11*10, 9*2 + 1});
    editor->setEditorStyle(AEditor::PasswordEditor);
    editor->setText("Password");
    editor->editorTextChanged->connect(new ASlot<AEditor*>(this, [](AEditor *editor) {
        std::cout << "password changed to " << editor->text() << std::endl;
    }));

    editor = new AEditor(this);
    editor->move({11*23, 9*7 });
    editor->resize({11*10, 9*18});
    editor->setEditorStyle(AEditor::MultilineEditor);
    editor->setText("Multiline Text Editor");
    editor->editorTextChanged->connect(new ASlot<AEditor*>(this, [](AEditor *editor) {
        std::cout << "multiline changed to " << editor->text() << std::endl;
    }));

    return true;
}

// paintEvent - sent during WM_PAINT, use this for decorating your widget.
void ExampleWindow::paintEvent()
{
    // Note that GDI resources aren't created until an APainter requests them
    // inside a set* call.  Likewise, changing a paint object's attributes
    // won't take effect until it is set into an APainter.  To create a GDI
    // object out of them yourself, create one with APaintHandle(AFont etc).
    // APaintHandle cleans up the GDI object when its destructor is called.

    // Create a painter object using this window's HWND.  You can also create
    // an APainter if you have a DC.
    APainter p(handle());

    // Set our bg color to the color of buttons, usually a light gray.
    AColor bg(GetSysColor(COLOR_BTNFACE));

    // Fill the background with the button color
    ABrush b(bg);
    p.setBrush(b);
    p.fillRect(ARect(0,0,size().width(),size().height()));

    // Draw some text beneath the column 1 controls
    AFont f("Segoe UI", 16, 400);  // font name, height, weight
    p.setBgColor(bg);
    p.setFgColor(AColor(0x5a, 0x5a, 0x5a));
    p.setFont(f);
    p.drawText(11*1, 9*26, "This text is drawn with the painting API.");
}

// This is invoked from inside ACheckBox's wmCommand function.
void ExampleWindow::check_checked(bool checked)
{
    std::cout << "button checked " << (checked ? "y" : "n") << std::endl;
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow)
{
    // Create an AApplication instance, this is responsible for the message
    // pump and processing deferred functions (called "pending functions").
    // There are constructors for argv, argc as well.
    AApplication app(hInstance, nCmdShow);

    // Create the example window.  Because this is deleted when it goes out of
    // scope, it has no parent object and we set the owner to nullptr.
    ExampleWindow window(nullptr);

    // Tell the application to terminate when the example window is destroyed.
    // Again the thing has no owner so we set that to nullptr.
    ASlot<> terminateMe(nullptr, [&]() {
        app.quit();
    });
    window.destroyed->connect(&terminateMe);

    // Run the message pump
    return app.run();

}
