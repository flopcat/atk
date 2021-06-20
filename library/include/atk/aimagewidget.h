#ifndef AIMAGEWIDGET_H
#define AIMAGEWIDGET_H

#include <atk/agraphics.h>
#include <atk/awidget.h>

class AImageWidget : public AWidget {
public:
    AImageWidget(AWidget *owner);

    virtual void paintEvent();

    void setBitmap(const ABitmapResource &bitmap);
    void setIcon(const AIconResource &icon);
private:
    ABitmapResource bitmap_;
    AIconResource icon_;
};

#endif /* AIMAGEWIDGET_H */
