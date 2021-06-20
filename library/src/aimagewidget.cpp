#include <atk/aimagewidget.h>



AImageWidget::AImageWidget(AWidget *owner) : AWidget(owner)
{
    setStyle(WS_CHILD | WS_VISIBLE);
}

void AImageWidget::paintEvent() {
    APainter p(handle());
    if (bitmap_) {
        p.drawBitmap({ 0, 0, size().width(), size().height() }, bitmap_);
        std::cout << "drew bitmap" << std::endl;
    } else if (icon_) {
        p.drawIcon({ 0, 0, size().width(), size().height() }, icon_);
    }
}

void AImageWidget::setBitmap(const ABitmapResource &bitmap)
{
    icon_.clear();
    bitmap_ = bitmap;
}

void AImageWidget::setIcon(const AIconResource &icon)
{
    bitmap_.clear();
    icon_ = icon;
}
