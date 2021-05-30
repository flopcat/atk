#ifndef ACONTROLHOST_H
#define ACONTROLHOST_H

#include <atk/awidget.h>
#include <windows.h>

class AControlHost : public AWidget
{
public:
    AControlHost(AWidget *parent);
    ~AControlHost();

protected:
    void setChildClass(const std::string &childClass);
    void setChildText(const std::string &childText);
    void setChildStyle(UINT style);
    HWND childHandle();
    const std::string &childText();

    bool createEvent();
    void resizeEvent();

    ASignal<> *childCreated;
    ASignal<> *aboutToDestroyChild;
    ASignal<> *childDestroyed;

    static const UINT defaultChildStyle;

private:
    AWidget *child;
    std::string childClass_;
    std::string childText_;
    UINT childStyle_;
};

#endif /* ACONTROLHOST_H */
