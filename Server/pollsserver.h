#ifndef POLLSSERVER_H
#define POLLSSERVER_H

#include "pch.h"
#include "dialog.h"


class PollsClient;
class PollsServer : public QTcpServer
{
    Q_OBJECT
public:
    PollsServer(Dialog *dialog, QObject *parent = 0);

    bool doStartServer(QHostAddress addr, qint16 port);
    bool isCodeAlreadyUsed(QString category, QString code);

public slots:
    void onRemoveUser(PollsClient *client);

protected:
    void incomingConnection(qintptr handle);

private:
//    QWidget *_widget;
    Dialog *_dialog;
    QList<PollsClient*> _clients;
};

#endif // POLLSSERVER_H
