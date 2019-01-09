#ifndef AWINDOW_H
#define AWINDOW_H

#include <awidget.h>

class AWindow : public AWidget
{
public:
    AWindow(AWidget *owner);

private:
    bool createEvent();
    void paintEvent();
};

#endif // AWINDOW_H
