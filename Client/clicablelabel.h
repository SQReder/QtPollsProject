#ifndef CLICABLELABEL_H
#define CLICABLELABEL_H

#include <QLabel>

class ClickableLabel : public QLabel
{
    Q_OBJECT
public:
    explicit ClickableLabel(QWidget *parent = 0);

signals:
    void clicked(ClickableLabel* who);

public slots:
    void slotClicked(ClickableLabel* who);

protected:
    void mousePressEvent (QMouseEvent *) ;
};

#endif // CLICABLELABEL_H
