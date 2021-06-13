#ifndef AWIDGET_H
#define AWIDGET_H

#include <string>
#include <functional>
#include <atk/aobject.h>
#include <atk/agraphics.h>
#include <windows.h>

class AControlHost;

class AWidget : public AObject
{
    enum DeadOrAlive { StillAlive, KilledBySystem, KilledByDestructor };
public:
    typedef bool (AWidget::*MessageFunc)(WPARAM,LPARAM,int&);
    typedef std::map<UINT, MessageFunc> MessageMap;

    AWidget(AWidget *owner = nullptr);
    virtual ~AWidget();

    void create();
    void show();
    void hide();
    void resize(const ASize size);
    void move(const APoint where);
    void setStyle(UINT style);
    void setExStyle(UINT exStyle);
    const std::string& text();
    void setText(const std::string &text);

    void setMessageFunction(UINT msg, MessageFunc func);
    template <class T>
    void setMessageFunction(UINT msg, bool (T::*func)(WPARAM,LPARAM,int&)) {
        setMessageFunction(msg, static_cast<MessageFunc>(func));
    }

    void addMessageMap(const MessageMap &map);

    // handle - HWND of this window
    HWND handle() { return handle_; }               // HWND of this window
    virtual HWND embedHandle() { return handle_; }  // HWND to embed from
    ASize size() const { return size_; }            // geometry (including frame)
    APoint position() const { return position_; }   // position (including frame)

    ASignal<> *aboutToCreate;
    ASignal<> *created;
    ASignal<> *aboutToDestroy;
    ASignal<> *destroyed;

protected:
    void setWindowClass(const std::string &windowClass);

    virtual bool createEvent();
    virtual bool closeEvent();
    virtual void destroyEvent();
    virtual void paintEvent();
    virtual void resizeEvent();

    static APaintHandle defaultFont;
    static std::string defaultWinClassName;
    static ATOM registerClass;
    static BOOL commonControls;
    std::string winClassName;
    MessageMap messageFunctions;

private:
    static LRESULT __stdcall wndProc(HWND hWnd, UINT id, WPARAM wParam, LPARAM lParam);

    HWND handle_;
    DeadOrAlive vitality;

    std::string text_;
    ASize size_;
    APoint position_;
    UINT style_;
    UINT exStyle_;

    bool visible;

    friend AControlHost;
};

#endif // AWIDGET_H
