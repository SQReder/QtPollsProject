#ifndef SCROLLGRID_H
#define SCROLLGRID_H

#include <QDialog>
#include <QMap>

namespace Ui {
class ScrollGrid;
}

class ClientConnection;
class ClickableLabel;
class ScrollGrid : public QDialog
{
    Q_OBJECT

public:
    explicit ScrollGrid(ClientConnection *client, QWidget *parent = 0);
    ~ScrollGrid();

    void LoadPics();
    QString _code;

protected:

signals:
    void hided();
    void OpenConfirm(QString filename);

private slots:
    virtual void showEvent(QShowEvent *);
    virtual void resizeEvent(QResizeEvent *);
    void hideEvent(QHideEvent *);

    void on_pushButton_clicked();
    void on_labelImage_clicked(ClickableLabel* who);

    void onVoteConfirmed(QString filename);
    void onVoteRejected();

private:
    Ui::ScrollGrid *ui;
    ClientConnection *_client;
    QMap<ClickableLabel*, QString> picToFile;
};

#endif // SCROLLGRID_H
