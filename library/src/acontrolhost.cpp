#include <atk/acontrolhost.h>

const UINT AControlHost::defaultChildStyle = WS_VISIBLE | WS_CHILD;

AControlHost::AControlHost(AWidget *owner) : AWidget(owner)
{
    setObjectName("Control Host Object");
    setStyle(defaultChildStyle);
    child = nullptr;
    // Create slots
    childCreated = new ASignal<>(this);
    aboutToDestroyChild = new ASignal<>(this);
    childDestroyed = new ASignal<>(this);
}

AControlHost::~AControlHost()
{
    aboutToDestroyChild->send();
    delete child;
    childDestroyed->send();
}

void AControlHost::setChildClass(const std::string &childClass)
{
    childClass_ = childClass;
}

void AControlHost::setChildText(const std::string &childText)
{
    childText_ = childText;
    if (child)
        child->setText(childText);
}

void AControlHost::setChildStyle(UINT style)
{
    childStyle_ = style;
    if (child)
        child->setStyle(style);
}

HWND AControlHost::childHandle()
{
    return child ? child->handle() : NULL;
}

const std::string &AControlHost::childText()
{
    return childText_;
}

bool AControlHost::sendChildMessage(UINT msg, WPARAM wParam, LPARAM lParam) {
    if (!child || !child->handle())
        return false;
    SendMessage(child->handle(), msg, wParam, lParam);
    return true;
}

bool AControlHost::createEvent()
{
    if (childClass_.empty()) {
        std::cerr << "Attempted to create child without setting its class!\n"
                  << "Object name = " << objectName() << "\n"
                  << "Text        = " << childText_ << "\n";
        abort();
    }
    std::cout << "Making child with " << childClass_ << " -> " << childText_ << "\n";

    child = new AWidget(this);
    child->setWindowClass(childClass_);
    child->setText(childText_);
    child->setStyle(childStyle_ | WS_CHILD | WS_VISIBLE);
    child->resize(size());
    child->created->connect(new ASlot<>(this, [this]() { childCreated->send(); }));
    child->show();
    return true;
}

void AControlHost::resizeEvent()
{
    if (!child)
        return;
    child->resize(size());
}



