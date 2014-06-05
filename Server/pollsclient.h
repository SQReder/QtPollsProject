#ifndef POLLSCLIENT_H
#define POLLSCLIENT_H

#include "pch.h"
#include "pollsserver.h"
#include "protocol.h"

class PollsClient : public QObject
{
    friend class PollsServer;
    Q_OBJECT
public:
    explicit PollsClient(qintptr desc, PollsServer *srv, QObject *parent = 0);

    bool GetAuthed() const { return _isAuthed; }
    void doSendCommand(qint8 comm);

    QString peerName();

signals:
    void addUserToGui(QString name);
    void removeUserFromGui(QString name);
    void removeUser(PollsClient *client);
    void doVoteUp(QString category, QString code, QString filename);

private slots:
    void onConnect();
    void onDisconnect();
    void onReadyRead();
    void onError(QAbstractSocket::SocketError socketError) const;
    void onTimerStop();

private:
    QTcpSocket *_sok;
    PollsServer *_srv;
    bool _authed;
    QString _name;
    qint64 _blockSize;
    bool _isAuthed;
    QTimer* pingTimer;
    QString _peerName;
};

#endif // POLLSCLIENT_H
