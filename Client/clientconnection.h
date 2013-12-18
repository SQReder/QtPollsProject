#ifndef CLIENTCONNECTION_H
#define CLIENTCONNECTION_H

#include <QObject>
#include <QTcpSocket>
#include <QColor>

class ClientConnection : public QObject
{
    Q_OBJECT
public:
    explicit ClientConnection(QObject *parent = 0);

    void doConnectToHost(QString addr, int port);
    void doDisconnect();
    void doSendCommand(qint8 command);
    void doSendCommand(qint8 command, QStringList data);

    void ValidateCode(QByteArray code);
signals:
    void AuthSuccess();
    void AddToLog(QString message, QColor color);
    void CodeVerified();
    void CodeNotVerified();
    void CodeAlreadyUsed();

private slots:
    void onSokConnected();
    void onSokDisconnected();
    void onSokReadyRead();
    void onSokDisplayError(QAbstractSocket::SocketError socketError);

private:
    QTcpSocket *_sok;
    qint64 _blockSize;
};

#endif // CLIENTCONNECTION_H
