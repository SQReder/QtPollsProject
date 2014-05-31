#ifndef DIALOG_H
#define DIALOG_H

#include "pch.h"

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

    bool isCodeAlreadyUsed(QString category, QString code);

private:
    Ui::Dialog *ui;
    QSharedPointer<PollsServer> _serv;
    QMap<QString, QString> _votes; // code to filename
    QMap<QString, QVector<QString>> _usedCodes; // code to categories
    QFile _logFile;
    short _port;

    void startServer();
    void loadConfig();
    void initializeLogging();

public slots:
    void onAddUserToGui(QString name);
    void onRemoveUserFromGui(QString name);
    void onMessageToGui(QString message, QString from);
    void onVoteUp(QString category, QString code, QString filename);

private slots:
    void on_pbStartStop_toggled(bool checked);
    void on_pushButton_clicked();
};

#endif // DIALOG_H
