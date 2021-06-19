#include <atk/ascrollbar.h>
#include <windows.h>

AScrollBar::AScrollBar(Orientation orient, AWidget *parent) : AControlHost(parent)
{
    setObjectName("ScrollBar Object");
    setChildClass("SCROLLBAR");
    setChildStyle(orient == HorizontalScroll ? SBS_HORZ : SBS_VERT);
    setMessageFunction(WM_HSCROLL, &AScrollBar::wmScroll);
    setMessageFunction(WM_VSCROLL, &AScrollBar::wmScroll);
    childCreated->connect(new ASlot<>(this, &AScrollBar::updateScrollInfo));
    positionChanged = new ASignal<int>(this);
    scrollBarPositionChanged = new ASignal<AScrollBar*,int>(this);
    minimum_ = 0;
    maximum_ = 100;
    orientation = orient;
    page_ = 10;
    value_ = 0;
}

int AScrollBar::maximum()
{
    return maximum_;
}

int AScrollBar::minimum()
{
    return minimum_;
}

int AScrollBar::optimumWidth()
{
    return GetSystemMetrics(SM_CXHSCROLL);
}

int AScrollBar::page()
{
    return page_;
}

int AScrollBar::value()
{
    return value_;
}


void AScrollBar::setMaximum(int value)
{
    maximum_ = value;
    value_ = std::min(value_, maximum_);
    updateScrollInfo();
}

void AScrollBar::setMinimum(int value)
{
    minimum_ = value;
    value_ = std::max(value_, minimum_);
    updateScrollInfo();
}

void AScrollBar::setPage(int amount)
{
    page_ = amount;
    updateScrollInfo();
}

void AScrollBar::setValue(int value)
{
    value_ = std::max(std::min(value, maximum_), minimum_);
    updateScrollPos();
}

void AScrollBar::updateScrollInfo()
{
    if (!childHandle())
        return;

    // Due to the way Windows decides the _scrollable_ area, we need to set the
    // maximum to include the page width.  Consider the case where we're
    // scrolling a bitmap: we can set si.nMax to the bitmap width and si.nPage
    // to the visible client width (this is evident from the online example
    // for bitmap scrolling).  So Windows will prevent you scrolling across the
    // area where the last page width would be.  Therefore, the page width
    // needs to be added to si.nMax to achieve the desired effect.
    SCROLLINFO si;
    si.cbSize = sizeof(si);
    si.fMask  = SIF_RANGE | SIF_PAGE | SIF_POS;
    si.nMin   = minimum_;
    si.nMax   = maximum_ + (page_ - 1);
    si.nPage  = page_;
    si.nPos = value_;
    SetScrollInfo(childHandle(), SB_CTL, &si, TRUE);

}

void AScrollBar::updateScrollPos()
{
    if (!childHandle())
        return;

    SCROLLINFO si = { 0 };
    si.cbSize = sizeof(si);
    si.fMask = SIF_POS;
    si.nPos = value_;
    SetScrollInfo(childHandle(), SB_CTL, &si, TRUE);
}

bool AScrollBar::wmScroll(WPARAM wParam, LPARAM lParam, int &ret)
{
    SCROLLINFO si = { 0 };
    // Get all the scroll bar information.
    si.cbSize = sizeof(SCROLLINFO);
    si.fMask  = SIF_ALL;

    GetScrollInfo((HWND)lParam, SB_CTL, &si);
    int xPos = value_;
    switch (LOWORD (wParam))
    {
    // User clicked the left arrow.
    case SB_LINELEFT:
        setValue(value_ - 1);
        break;

    // User clicked the right arrow.
    case SB_LINERIGHT:
        setValue(value_ + 1);
        break;

    // User clicked the scroll bar shaft left of the scroll box.
    case SB_PAGELEFT:
        setValue(value_ - page_);
        break;

    // User clicked the scroll bar shaft right of the scroll box.
    case SB_PAGERIGHT:
        setValue(value_ + page_);
        break;

    // User dragged the scroll box.
    case SB_THUMBTRACK:
        setValue(si.nTrackPos);
        break;

    default :
        break;
    }
    if (value_ != xPos) {
        positionChanged->send(value_);
        scrollBarPositionChanged->send(this, value_);
    }
    ret = 0;
    return true;
}
