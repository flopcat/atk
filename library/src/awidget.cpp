#include <assert.h>
#include <iostream>
#include <windows.h>
#include <commctrl.h>
#include "atk/utf8.h"
#include "atk/awidget.h"
#include "atk/agraphics.h"
#include "atk/aapplication.h"

APaintHandle AWidget::defaultFont(AFont("Segoe UI", 16));

std::string AWidget::defaultWinClassName("AWidgetClass");

ATOM AWidget::registerClass = []() -> ATOM {
    WNDCLASSEX winClass = { sizeof(WNDCLASSEX), 0, &AWidget::wndProc, 0, sizeof(AWidget*),
                            GetModuleHandle(0), 0,
                            LoadCursor(nullptr, IDC_ARROW),
                            (HBRUSH)COLOR_BTNFACE+1, nullptr,
                            L"AWidgetClass", 0 };
    ATOM a = RegisterClassEx(&winClass);
        std::cout << "registered window class " << defaultWinClassName << ": " << a << std::endl;
    return a;
}();

BOOL commonControls = []() -> BOOL {
    INITCOMMONCONTROLSEX icce;
    icce.dwSize = sizeof(icce);
    icce.dwICC = ICC_WIN95_CLASSES;
    return InitCommonControlsEx(&icce);
}();

AWidget::AWidget(AWidget *owner) : AObject(owner)
{
    handle_ = nullptr;
    size_ = ASize(640, 480);
    position_ = APoint(0, 0);
    vitality = StillAlive;
    visible = false;
    winClassName = defaultWinClassName;

    aboutToCreate = new ASignal<>(this);
    created = new ASignal<>(this);
    aboutToDestroy = new ASignal<>(this);
    destroyed = new ASignal<>(this);
    aApp->addPendingFunction(new ASlot<>(this, &AWidget::create));
}

AWidget::~AWidget()
{
    if (vitality == StillAlive) {
        vitality = KilledByDestructor;
        DestroyWindow(handle_);
    }
}

void AWidget::setWindowClass(const std::string &windowClass)
{
    winClassName = windowClass;
}

void AWidget::create()
{
    std::cout << "create called" << winClassName << std::endl;
    if (RegisterClass == RegisterClassW) {
        std::cout << "unicode mode" << std::endl;
    }
    if (winClassName.empty()) {
        std::cerr << "Attempted to create widget without setting its class!\n"
                  << "Object name = " << objectName() << "\n"
                  << "Text        = " << text_ << "\n";
        abort();
    }
    bool isCustomClass = winClassName != defaultWinClassName;
    if (isCustomClass)
        aboutToCreate->send();
    AWidget *owner = this->parent() ? static_cast<AWidget*>(this->parent()) : nullptr;
    handle_ = CreateWindow(widen(winClassName).c_str(),
                   widen(text_).c_str(),
                   style_,
                   position_.x(), position_.y(),
                   size_.width(), size_.height(),
                   owner ? owner->embedHandle() : nullptr,
                   0, aApp->handle(),
                   this);
    if (!handle_) {
        std::cout << GetLastError() << std::endl;
    }
    assert(handle_ != nullptr);
    SendMessage(handle_, WM_SETFONT, (WPARAM)defaultFont.handle(), 0);
    if (isCustomClass)
        created->send();
    std::cout << "create finished" << std::endl;
}

void AWidget::show()
{
    std::cout << "show called" << std::endl;
    aApp->addPendingFunction(new ASlot<>(this, [this]() {
        std::cout << "show made" << std::endl;
        ShowWindow(handle_, SW_SHOW);
        UpdateWindow(handle_);
    }));
    visible = true;
}

void AWidget::hide()
{
    ShowWindow(handle_, SW_HIDE);
    visible = false;
}

void AWidget::resize(const ASize size)
{
    auto func = [this,size]() {
        SetWindowPos(handle_, 0, 0, 0, size.width(), size.height(),
                     SWP_ASYNCWINDOWPOS | SWP_NOOWNERZORDER | SWP_NOMOVE
                     | (visible ? SWP_SHOWWINDOW : 0));
    };
    if (!handle_) {
        //aApp->addPendingFunction(new ASlot<>(this, func));
        size_ = size;
    } else {
        func();
    }
}

void AWidget::move(const APoint where)
{
    auto func = [this,where]() {
        SetWindowPos(handle_, 0, where.x(), where.y(), 0, 0,
                     SWP_ASYNCWINDOWPOS | SWP_NOOWNERZORDER | SWP_NOSIZE
                     | (visible ? SWP_SHOWWINDOW : 0));
    };
    if (!handle_) {
        position_ = where;
        //aApp->addPendingFunction(new ASlot<>(this, func));
    } else {
        func();
    }
}

void AWidget::setStyle(UINT style)
{
    if (handle_) {
        SetWindowLong(handle_, GWL_STYLE, style);
        SetWindowPos(handle_, 0, 0, 0, 0, 0,
                     SWP_ASYNCWINDOWPOS | SWP_NOOWNERZORDER | SWP_NOMOVE
                     | SWP_NOSIZE | SWP_FRAMECHANGED | (visible ?: SWP_SHOWWINDOW));
    }
    style_ = style;
}

const std::string& AWidget::text()
{
    return text_;
}

void AWidget::setText(const std::string &text)
{
    text_ = text;
    if (handle_) {
        SetWindowText(handle_, widen(text_).c_str());
    }
}

LRESULT __stdcall AWidget::wndProc(HWND hWnd, UINT id, WPARAM wParam, LPARAM lParam)
{
    int ret = 0;
    bool parsed = false;
    AWidget *self = reinterpret_cast<AWidget*>(GetWindowLongPtr(hWnd, 0));

    //std::cout << id << std::endl;
    if (id == WM_NCCREATE) {
        CREATESTRUCT *createArgs = reinterpret_cast<CREATESTRUCT*>(lParam);
        self = static_cast<AWidget*>(createArgs->lpCreateParams);
        SetWindowLongPtr(hWnd, 0, reinterpret_cast<LONG_PTR>(self));
        std::cout << "set handle to " << hWnd;
        self->handle_ = hWnd;
        self->aboutToCreate->send();
    }
    if (!self)
        goto last;

    switch (id) {
    case WM_NCCREATE: {
        break;
    }
    case WM_CREATE: {
        bool ok = self->createEvent();
        if (ok)
            self->created->send();
        return ok ? 0 : 1;
    }
    case WM_CLOSE:
        if (self->closeEvent())
            DestroyWindow(self->handle_);
        return 0;
    case WM_DESTROY:
        if (self->vitality != KilledByDestructor)
            self->vitality = KilledBySystem;
        self->aboutToDestroy->send();
        self->destroyEvent();
        self->bereave();
        return 0;
    case WM_ERASEBKGND: {
        APainter p((HDC)wParam);
        AColor bg(GetSysColor(COLOR_BTNFACE));
        ABrush b(bg);
        p.setBrush(b);
        auto sz = self->size();
        p.fillRect(ARect(0,0,sz.width(),sz.height()));
        std::cout << "erased background" << std::endl;
        return 1;
    }
    case WM_PAINT:
        self->paintEvent();
        return 0;
    case WM_SIZE: {
        self->size_ = ASize(LOWORD(lParam),HIWORD(lParam));
        self->resizeEvent();
        return 0;
    }
    }
    if (self && self->messageFunctions.count(id) != 0) {
        std::cout << "handing off call to message function" << std::endl;
        auto func = self->messageFunctions.at(id);
        parsed = (self->*func)(wParam, lParam, ret);
    }
    if (id == WM_NCDESTROY) {
        self->destroyed->send();
        SetWindowLongPtrA(hWnd, 0, 0);
        if (self->vitality == KilledBySystem && self->parent())
            delete self;
    }
    last:
    return parsed ? ret : DefWindowProc(hWnd, id, wParam, lParam);
}

void AWidget::setMessageFunction(UINT msg, MessageFunc func)
{
	auto it = messageFunctions.find(msg);
	if (it != messageFunctions.end())
		messageFunctions.erase(it);
    messageFunctions.insert({msg, func});
}

void AWidget::addMessageMap(const AWidget::MessageMap &map)
{
    messageFunctions.insert(map.begin(), map.end());
}

bool AWidget::createEvent()
{
    return true;
}

bool AWidget::closeEvent()
{
    return true;
}

void AWidget::destroyEvent()
{
    bereave();
}

void AWidget::paintEvent()
{
    APainter p(this->handle());
    ABrush back(GetSysColor(COLOR_BTNFACE));
    p.setBrush(back);
    p.fillRect(ARect(0, 0, size_.width(), size_.height()));
}

void AWidget::resizeEvent()
{

}

