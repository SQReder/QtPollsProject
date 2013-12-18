#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QTcpSocket>
#include <QHostAddress>

#include "clientconnection.h"

class ScanWindow;
namespace Ui {
class Dialog;
}

class ConnectDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConnectDialog(QWidget *parent = 0);
    ~ConnectDialog();

public slots:
    void onAddToLog(QString text, QColor color = Qt::black);

private slots:
    void onAuthSuccess();
    void on_pbConnect_toggled(bool checked);

    void on_pbSelectFolder_clicked();

private:
    Ui::Dialog *ui;
    ScanWindow *_scanWindow;
    ClientConnection *_client;
};

extern QString picsPath;
#endif // DIALOG_H
