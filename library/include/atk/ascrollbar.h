#ifndef ASCROLLBAR_H
#define ASCROLLBAR_H

#include <atk/acontrolhost.h>

class AScrollBar : public AControlHost
{
public:
    enum Orientation { HorizontalScroll, VerticalScroll };
    AScrollBar(Orientation orient, AWidget *parent);

    int maximum();
    int minimum();
    static int optimumWidth();
    int page();
    int value();

    void setMaximum(int value);
    void setMinimum(int value);
    void setPage(int amount);
    void setValue(int value);

    ASignal<int> *positionChanged;
    ASignal<AScrollBar*, int> *scrollBarPositionChanged;

protected:
    void updateScrollInfo();
    void updateScrollPos();
    bool wmScroll(WPARAM wParam, LPARAM lParam, int &ret);

private:
    Orientation orientation;
    int minimum_;
    int maximum_;
    int page_;
    int value_;
};

class AHorizontalScrollBar : public AScrollBar {
public:
    AHorizontalScrollBar(AWidget *parent) :
        AScrollBar(AScrollBar::HorizontalScroll, parent) {
        setObjectName("Horizontal ScrollBar Object");
    }
};

class AVerticalScrollBar : public AScrollBar {
public:
    AVerticalScrollBar(AWidget *parent) :
        AScrollBar(AScrollBar::VerticalScroll, parent) {
        setObjectName("Vertical ScrollBar Object");
    }
};

#endif // ASCROLLBAR_H
