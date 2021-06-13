#include <iostream>
#include <atk/aeditor.h>

AEditor::AEditor(AWidget *parent) :
    AControlHost(parent)
{
    setChildText("Editor");
    setChildClass("EDIT");
    setChildStyle(WS_BORDER);
    setChildExStyle(WS_EX_CLIENTEDGE);
    setMessageFunction(WM_COMMAND, &AEditor::wmCommand);

    textChanged = new ASignal<>(this);
    editorTextChanged = new ASignal<AEditor*>(this);
}

const std::string &AEditor::text()
{
    updateChildText();
    return childText();
}

void AEditor::setText(const std::string &text)
{
    setChildText(text);
}

void AEditor::setEditorStyle(EditorStyle editor)
{
    UINT style;
    switch (editor) {
    case LineEditor:
        style = 0;
        break;
    case PasswordEditor:
        style = ES_PASSWORD;
        break;
    case MultilineEditor:
        style = ES_MULTILINE;
        break;
    }
    setChildStyle(WS_BORDER | style);
}

void AEditor::setThinBorder(bool thin)
{
    setChildExStyle(thin ? 0 : WS_EX_CLIENTEDGE);
}

bool AEditor::wmCommand(WPARAM wParam, LPARAM lParam, int &ret)
{
    if (HIWORD(wParam) == EN_UPDATE) {
        textChanged->send();
        editorTextChanged->send(this);
        return true;
    }
    return false;
}
