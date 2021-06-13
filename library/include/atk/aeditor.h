#ifndef AEDITOR_H
#define AEDITOR_H

#include <atk/acontrolhost.h>

class AEditor : public AControlHost
{
public:
    enum EditorStyle { LineEditor, PasswordEditor, MultilineEditor };

    AEditor(AWidget *parent);

    const std::string &text();
    void setText(const std::string &text);
    void setEditorStyle(EditorStyle editor);
    void setThinBorder(bool thin);

    ASignal<> *textChanged;
    ASignal<AEditor*> *editorTextChanged;

protected:
    bool wmCommand(WPARAM wParam, LPARAM lParam, int &ret);
};




#endif // AEDITOR_H
