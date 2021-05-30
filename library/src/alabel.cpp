#include <atk/alabel.h>

ALabel::ALabel(AWidget *owner) : AControlHost(owner)
{
    setObjectName("Button Object");

    setChildClass("STATIC");
    setChildText("Label");
}

const std::string &ALabel::text()
{
    return childText();
}

void ALabel::setText(const std::string &text)
{
    setChildText(text);
}

