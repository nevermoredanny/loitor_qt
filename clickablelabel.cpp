#include "clickablelabel.h"

ClickableLabel::ClickableLabel(QWidget* parent)
    : QLabel(parent)
{

}

ClickableLabel::~ClickableLabel()
{

}

void ClickableLabel::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        emit clicked_left();
    }

    if(event->button() == Qt::RightButton)
    {
        emit clicked_right();
    }

    QLabel::mousePressEvent(event);
}
