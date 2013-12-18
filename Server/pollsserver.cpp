#include <QHostAddress>
#include "pollsserver.h"
#include "pollsclient.h"
#include "dialog.h"

PollsServer::PollsServer(QWidget *widget, QObject *parent):
    QTcpServer(parent),
    _widget(widget)
{
}

bool PollsServer::doStartServer(QHostAddress addr, qint16 port)
{
    if (!listen(addr, port))
    {
        qDebug() << "Server not started at" << addr << ":" << port;
        return false;
    }
    qDebug() << "Server started at" << addr << ":" << port;
    return true;
}

void PollsServer::incomingConnection(int handle)
{
    //создаем клиента
    PollsClient *client = new PollsClient(handle, this);
    if (_widget != 0)
    {
        connect(client, SIGNAL(addUserToGui(QString)), _widget, SLOT(onAddUserToGui(QString)));
        connect(client, SIGNAL(removeUserFromGui(QString)), _widget, SLOT(onRemoveUserFromGui(QString)));
        connect(client, SIGNAL(onAddLogToGui(QString,QColor)), _widget, SLOT(onAddLogToGui(QString,QColor)));
    }
    connect(client, SIGNAL(removeUser(PollsClient*)), this, SLOT(onRemoveUser(PollsClient*)));
    connect(client, SIGNAL(doVoteUp(QString,QString)), _widget, SLOT(onVoteUp(QString,QString)));
    _clients.append(client);
}

void PollsServer::onRemoveUser(PollsClient *client)
{
    _clients.removeAt(_clients.indexOf(client));
}

bool PollsServer::isCodeAlreadyUsed(QString code) {
    Dialog* d = dynamic_cast<Dialog*>(_widget);
    return d->isCodeAlreadyUsed(code);
}
