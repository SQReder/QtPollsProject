#ifndef POLLSSERVER_H
#define POLLSSERVER_H

#include <QColor>
#include <QTcpServer>
#include <QtWidgets>

class PollsClient;
class PollsServer : public QTcpServer
{
    Q_OBJECT
public:
    PollsServer(QWidget *widget, QObject *parent = 0);

    bool doStartServer(QHostAddress addr, qint16 port);
    bool isCodeAlreadyUsed(QString code);

public slots:
    void onRemoveUser(PollsClient *client);

signals:
    void addLogToGui(QString string, QColor color);

protected:
    void incomingConnection(qintptr handle);

private:
    QWidget *_widget;
    QList<PollsClient*> _clients;
};

#endif // POLLSSERVER_H
