#ifndef SCANWINDOW_H
#define SCANWINDOW_H

#include <QDialog>
#include <scanworker.h>
#include "scrollgrid.h"

class ClientConnection;
namespace Ui {
class ScanWindow;
}

class ScanWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ScanWindow(ClientConnection *client, QWidget *parent = 0);
    ~ScanWindow();

private slots:
    void on_pushButton_clicked();
    void onCodeScanned(QString code);
    void onCodeVerified();
    void onCodeNotVerified();
    void onCodeAlreadyUsed();

    void on_pbSendCode_clicked();
    void onTimerStop();

protected:
    void closeEvent(QCloseEvent *);
    void keyPressEvent(QKeyEvent *);
    void showEvent(QShowEvent *);

private:
    Ui::ScanWindow *ui;
    ClientConnection *_client;
    bool canBeClosed;
    QThread *_scanThread;

    ScrollGrid *picWindow;
    QString lastCode;

    bool waitCodeApprovement;
};

#endif // SCANWINDOW_H
