#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QMap>
#include <QFile>
#include <QDataStream>

class QFile;

class PollsServer;

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

    bool isCodeAlreadyUsed(QString code);

private:
    Ui::Dialog *ui;
    PollsServer *_serv;
    void addToLog(QString text, QColor color = Qt::darkGray);
    QMap<QString, QString> _votes;
    QFile _logFile;
    short _port;

    void startServer();

public slots:
    void onAddUserToGui(QString name);
    void onRemoveUserFromGui(QString name);
    void onMessageToGui(QString message, QString from);
    void onAddLogToGui(QString string, QColor color);
    void onVoteUp(QString code, QString filename);

private slots:
    void on_pbStartStop_toggled(bool checked);

};

#endif // DIALOG_H
