#include "clicablelabel.h"

ClickableLabel::ClickableLabel(QWidget *parent) :
    QLabel(parent)
{
    connect( this, SIGNAL( clicked(ClickableLabel*) ), this, SLOT( slotClicked(ClickableLabel*) ) );
}

void ClickableLabel::slotClicked(ClickableLabel*)
{
//    qDebug()<<"Clicked";
}

void ClickableLabel::mousePressEvent ( QMouseEvent *)
{
    emit clicked(this);
}
