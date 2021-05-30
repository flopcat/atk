#include <atk/aapplication.h>
#include <atk/awindow.h>
#include <atk/abutton.h>
#include <atk/agraphics.h>
#include <iostream>
#include <atk/utf8.h>
#include <windows.h>

AWindow::AWindow(AWidget *owner) : AWidget(owner)
{
    setObjectName("Window Object");
    setStyle(WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_SYSMENU);
}

