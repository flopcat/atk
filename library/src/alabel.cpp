#include <atk/alabel.h>

ALabel::ALabel(AWidget *owner) : AControlHost(owner)
{
    setObjectName("Label Object");

    setChildClass("STATIC");
    setChildText("Label");
    setMessageFunction(WM_CTLCOLORSTATIC, &ALabel::wmCtlColorStatic);
}

const std::string &ALabel::text()
{
    return childText();
}

void ALabel::setText(const std::string &text)
{
    setChildText(text);
}

void ALabel::setFrameEdge(FrameEdge edge)
{
	UINT style = AControlHost::defaultChildStyle;
	switch(edge) {
	case ALabel::NoFrame:
		break;
	case ALabel::GrayFrame:
		style |= SS_GRAYFRAME;
		break;
	case ALabel::WhiteFrame:
		style |= SS_WHITEFRAME;
		break;
	case ALabel::BlackFrame:
		style |= SS_BLACKFRAME;
		break;
	case ALabel::SunkenFrame:
		style |= SS_SUNKEN;
		break;
	case ALabel::EtchedFrame:
		style |= SS_ETCHEDFRAME;
		break;
	}
	setChildStyle(style);
}

void ALabel::setColor(const AColor &color)
{
    this->color = color;
}

bool ALabel::wmCtlColorStatic(WPARAM wParam, LPARAM lParam, int &ret)
{
    if (color.isClear())
        return false;
    APainter p((HDC)wParam);
    p.setFgColor(color);
    p.setBgColor(GetSysColor(COLOR_BTNFACE));
    ret = (int)GetSysColorBrush(COLOR_BTNFACE);
    return true;
}
