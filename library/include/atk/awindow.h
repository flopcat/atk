#ifndef AWINDOW_H
#define AWINDOW_H

#include <atk/awidget.h>

class AWindow : public AWidget
{
public:
    AWindow(AWidget *owner);
};

#endif // AWINDOW_H
