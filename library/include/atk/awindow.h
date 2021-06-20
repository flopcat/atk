#ifndef AWINDOW_H
#define AWINDOW_H

#include <atk/agraphics.h>
#include <atk/awidget.h>

class AWindowMetrics
{
public:
    static int caption();
    static int frame();
    static int sizeFrame();
};

class AWindow : public AWidget
{
public:
    AWindow(AWidget *owner);
    void setIcon(const AIconResource &icon);

    AWindowMetrics metrics;

private:
    void realSetIcon(HICON icon);
    void self_created();
    AIconResource windowIcon;
};

#endif // AWINDOW_H
