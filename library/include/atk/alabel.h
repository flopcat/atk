/*
 * alabel.h
 *
 *  Created on: 30 May 2021
 *      Author: Anon
 */
#ifndef ALABEL_H
#define ALABEL_H
#include <atk/awidget.h>
#include <atk/acontrolhost.h>


class ALabel : public AControlHost
{
public:
	enum FrameEdge { NoFrame, GrayFrame, WhiteFrame, BlackFrame, SunkenFrame, EtchedFrame };

    ALabel(AWidget *parent);
    const std::string &text();
    void setText(const std::string &text);
    void setFrameEdge(FrameEdge edge);
};

#endif // ALABEL_H
