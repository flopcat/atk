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
    ALabel(AWidget *parent);
    const std::string &text();
    void setText(const std::string &text);
};

#endif // ALABEL_H
