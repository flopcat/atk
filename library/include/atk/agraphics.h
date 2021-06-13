#ifndef AGRAPHICS_H
#define AGRAPHICS_H
#include <string>
#include <atk/aapplication.h>
#include <atk/utf8.h>
#include <windows.h>

class APaintObject;
class APen;
class ABrush;
class AFont;
class ABitmap;
class ARegion;
class APainter;

class AColor {
public:
    AColor() : q({0,0,0,0}) {}
    AColor(const AColor &other) { q = other.q; }
    AColor(COLORREF color) { setColor(color); }
    AColor(int r, int g, int b) { q = { (BYTE)b, (BYTE)g, (BYTE)r, 255 }; }
    COLORREF color() const { return RGB(q.rgbRed, q.rgbGreen, q.rgbBlue); }
    BYTE red() const { return q.rgbRed; }
    BYTE green() const { return q.rgbGreen; }
    BYTE blue() const { return q.rgbBlue; }
    bool isClear() const { return q.rgbReserved == 0; }
    void setColor(COLORREF ref) { q = { GetRValue(ref), GetGValue(ref), GetBValue(ref), 255 }; }
    void setColor(int r, int g, int b) { q = { (BYTE)b, (BYTE)g, (BYTE)r, 255 }; }
    void setRed(BYTE r) { q.rgbRed = r; q.rgbReserved = 255; }
    void setGreen(BYTE g) { q.rgbGreen = g; q.rgbReserved = 255; }
    void setBlue(BYTE b) { q.rgbBlue = b; q.rgbReserved = 255; }
    void clear() { q = { 0, 0, 0, 0 }; }
private:
    RGBQUAD q;
};

class APoint {
public:
    APoint() : p({0,0}) {}
    APoint(const APoint &other) : p(other.p) {}
    APoint(int x, int y) : p({x,y}) {}
    const POINT *point() const { return &p; }
    int x() const { return p.x; }
    int y() const { return p.y; }
    void setX(int x) { p.x = x; }
    void setY(int y) { p.y = y; }
    void translate(int dx, int dy) { p.x += dx; p.y += dy; }
private:
    POINT p;
};

class ASize {
public:
    ASize() : sz({0,0}) {}
    ASize(int w, int h) : sz({w, h}) {}
    ASize(const ASize &other) : sz(other.sz) {}
    const SIZE *size() const { return &sz; }
    int width() const { return sz.cx; }
    int height() const { return sz.cy; }
    void setWidth(int w) { sz.cx = w; }
    void setHeight(int h) { sz.cy = h; }
    void shrink(int dw, int dh) { sz.cx -= dw; sz.cy -= dh; }
    void grow(int dw, int dh) { sz.cx += dw; sz.cy += dh; }
private:
    SIZE sz;
};

class ARect {
public:
    ARect() : rc({0,0,0,0}) {}
    ARect(const ARect &other) : rc(other.rc) {}
    ARect(const RECT &other) : rc(other) {}
    ARect(int x, int y, int w, int h) { rc = { x, y, x+w, x+h }; }
    const RECT *rect() const { return &rc; }
    void adjust(int dx, int dy) { rc.left += dx; rc.right += dx;
                                  rc.top += dy; rc.bottom += dy; }
    void shrink(int c) { rc.left += c; rc.right -= c;
                         rc.top += c; rc.bottom -= c; }
    void grow(int c) { rc.left -= c; rc.right += c;
                       rc.top -= c; rc.bottom += c; }
    int left() const { return rc.left; }
    int top() const { return rc.top; }
    int right() const { return rc.right; }
    int bottom() const { return rc.bottom; }
    int width() const { return rc.right - rc.left; }
    int height() const { return rc.bottom - rc.top; }
private:
    RECT rc;
};
class APaintObject {
public:
    APaintObject() {}
    virtual ~APaintObject() { }
    virtual HGDIOBJ makeNativeHandle() const = 0;
};

class APen : public APaintObject {
public:
    APen(const AColor &c = AColor(), int width = 0) : APaintObject(), color_(c), width_(width) {}
    HGDIOBJ makeNativeHandle() const {
        return CreatePen(PS_SOLID, width_, color_.color());
    }
    AColor color() { return color_; }
    void setColor(const AColor &color) { color_ = color; }
    int width() { return width_; }
    void setWidth(int width) { width_ = width; }

private:
    AColor color_;
    int width_;
};

class ABrush : public APaintObject {
public:
    ABrush(const AColor &c = AColor()) : APaintObject(), color_(c) {}
    HGDIOBJ makeNativeHandle() const {
        return CreateSolidBrush(color_.color());
    }
    AColor color() { return color_; }
    void setColor(const AColor &color) { color_ = color; }

private:
    AColor color_;
};

class AFont : public APaintObject {
public:
    AFont(const std::string &name = "Microsoft Sans Serif", int height = 16,
          int weight = 400, bool italic = false, bool underline = false)
        : APaintObject(), name_(name), height_(height), weight_(weight),
        italic_(italic), underline_(underline) {}
    HGDIOBJ makeNativeHandle() const {
        return CreateFont(height_, 0, 0, 0, weight_, italic_, underline_, 0,
                ANSI_CHARSET, 0, 0, 0, CLEARTYPE_QUALITY, widen(name_).c_str());
    }
    std::string name() const { return name_; }
    const std::string& nameR() const { return name_; }
    void setName(const std::string &name) { name_ = name; }
    int height() { return height_; }
    void setHeight(int height) { height_ = height; }
    int weight() { return weight_; }
    void setWeight(int weight) { weight_ = weight; }
    bool italic() { return italic_; }
    void setItalic(bool italic) { italic_ = italic; }
    bool underline() { return underline_; }
    void setUnderline(bool underline) { underline_ = underline; }

private:
    std::string name_;
    int height_;
    int weight_;
    bool italic_;
    bool underline_;
};

class ABitmap : public APaintObject {
public:
    ABitmap(int width = 16, int height = 16)
        : APaintObject(), width_(width), height_(height) {}
    HGDIOBJ makeNativeHandle() const {
        return CreateBitmap(width_, height_, 3, 8, NULL);
    }
    int width() { return width_; }
    void setWidth(int width) { width_ = width; }
    int height() { return height_; }
    void setHeight(int height) { height_ = height; }
private:
    int width_;
    int height_;
};

template <typename T, WINBOOL (*WINAPI dtor)(T)>
class APaintHandle_T {
public:
    explicit APaintHandle_T(const APaintObject &obj) {
        h = obj.makeNativeHandle();
    }
    explicit APaintHandle_T(T handle) {
        h = handle;
    }
    virtual ~APaintHandle_T() {
        if (!h)
            return;
        dtor(h);
        h = nullptr;
    }
    T handle() { return h; }
protected:
    T h = nullptr;
};

typedef APaintHandle_T<HGDIOBJ, &DeleteObject> APaintHandle;

template<typename T, UINT type, WINBOOL (*WINAPI dtor)(T)>
class APaintResource
{
public:
    APaintResource() {}
    APaintResource(UINT resourceId) : resourceId(resourceId) {}
    APaintResource(std::string fileName) : fileName(fileName) {}
    APaintResource(const APaintResource<T,type,dtor> &h) {
        resourceId = h.resourceId;
        fileName = h.fileName;
        handle_ = CopyImage(h.handle_, type, 0, 0, LR_COPYFROMRESOURCE);
    }
    APaintResource(APaintResource<T,type,dtor> &&h) {
        resourceId = std::move(h.resourceId);
        fileName = std::move(h.fileName);
        handle_ = std::move(h.handle_);
        h.resourceId = 0;
        h.handle_ = NULL;
    }
    virtual ~APaintResource() {
        if (handle_)
            dtor(handle_);
    }
    T handle() {
        if (!handle_)
            load();
        return handle_;
    }
    void clear() {
        if (handle_) {
            dtor(handle_);
            handle_ = NULL;
        }
    }
protected:
    UINT resourceId = 0;
    std::string fileName;
    T handle_;

    void load() {
        if (resourceId)
            handle_ = LoadImage(aApp->handle(), MAKEINTRESOURCE(resourceId), type, 0, 0, LR_SHARED);
        else
            handle_ = LoadImage(aApp->handle(), widen(fileName).c_str(), type, 0, 0, LR_LOADFROMFILE | LR_SHARED);
    }
};

typedef APaintResource<HGDIOBJ, IMAGE_BITMAP, DeleteObject> ABitmapResource;
typedef APaintResource<HCURSOR, IMAGE_CURSOR, DestroyCursor> ACursorResource;
typedef APaintResource<HICON, IMAGE_ICON, DestroyIcon> AIconResource;

class APainter {
public:
    APainter(HWND widgetHandle) : hWnd(widgetHandle),
        orgPen(0), orgBrush(0), orgFont(0), orgBitmap(0),
        orgRegion(0), nowBrush(0) { hdc = BeginPaint(hWnd, &ps); }
    APainter(HDC surface) : hWnd(NULL), hdc(surface),
        orgPen(0), orgBrush(0), orgFont(0), orgBitmap(0),
        orgRegion(0), nowBrush(0) { }
    ~APainter() {
        if (orgPen) DeleteObject(SelectObject(hdc, orgPen));
        if (orgBrush) DeleteObject(SelectObject(hdc, orgBrush));
        if (orgFont) DeleteObject(SelectObject(hdc, orgFont));
        if (orgBitmap) DeleteObject(SelectObject(hdc, orgBitmap));
        if (orgRegion) DeleteObject(SelectObject(hdc, orgRegion));
        if (hWnd) EndPaint(hWnd, &ps); }
    HDC handle() const { return hdc; }
    void setBrush(const ABrush &brush) {
        nowBrush = brush.makeNativeHandle();
        HGDIOBJ b = SelectObject(hdc, nowBrush);
        if (!orgPen)
            orgPen = b;
        else
            DeleteObject(b);
    }
    void setPen(const APen &pen) {
        HGDIOBJ p = SelectObject(hdc, pen.makeNativeHandle());
        if (!orgPen)
            orgPen = p;
        else
            DeleteObject(p);
    }
    void setFont(const AFont &font) {
        HGDIOBJ f = SelectObject(hdc, font.makeNativeHandle());
        if (!orgFont)
            orgFont = f;
        else
            DeleteObject(f);
    }
    void setBitmap(const ABitmap &bitmap) {
        HGDIOBJ b = SelectObject(hdc, bitmap.makeNativeHandle());
        if (!orgBitmap)
            orgBitmap = b;
        else
            DeleteObject(b);
    }

    void setBgColor(const AColor &c) { SetBkColor(hdc, c.color()); }
    void setFgColor(const AColor &c) { SetTextColor(hdc, c.color()); }

    void fillRect(const ARect &rect) { FillRect(hdc, rect.rect(), (HBRUSH)nowBrush); }
    void drawFrame(const ARect &rect) { FrameRect(hdc, rect.rect(), (HBRUSH)nowBrush); }
    void drawRect(const ARect &rect) { Rectangle(hdc, rect.left(), rect.top(), rect.right(), rect.bottom()); }
    void drawEllipse(const ARect &rect) { Ellipse(hdc, rect.left(), rect.top(), rect.right(), rect.bottom()); }
    void drawText(int x, int y, const std::string text) { std::wstring w = widen(text); TextOut(hdc, x, y, w.c_str(), w.length()-1); }

private:
    HWND hWnd;
    HDC hdc;
    PAINTSTRUCT ps;
    HGDIOBJ orgPen;
    HGDIOBJ orgBrush;
    HGDIOBJ orgFont;
    HGDIOBJ orgBitmap;
    HGDIOBJ orgRegion;
    HGDIOBJ nowBrush;
};

#endif // AGRAPHICS_H
