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
    created->connect(new ASlot<>(this, &AWindow::self_created));
}

void AWindow::setIcon(const AIconResource &icon)
{
    windowIcon = icon;
    realSetIcon(windowIcon.handle());
}

void AWindow::realSetIcon(HICON icon)
{
    if (handle()) {
        SendMessage(handle(), WM_SETICON, ICON_SMALL, (LPARAM)icon);
        SendMessage(handle(), WM_SETICON, ICON_BIG, (LPARAM)icon);
    }
}

void AWindow::self_created()
{
    if (windowIcon)
        realSetIcon(windowIcon.handle());
}
