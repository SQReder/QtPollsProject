#include <QHostAddress>
#include "pollsserver.h"
#include "pollsclient.h"

PollsServer::PollsServer(Dialog *dialog, QObject *parent):
    QTcpServer(parent),
    _dialog(dialog)
{ }

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
    if (_dialog != 0)
    {
        connect(client, SIGNAL(addUserToGui(QString)), _dialog, SLOT(onAddUserToGui(QString)));
        connect(client, SIGNAL(removeUserFromGui(QString)), _dialog, SLOT(onRemoveUserFromGui(QString)));
        connect(client, SIGNAL(doVoteUp(QString,QString,QString,QString)), _dialog, SLOT(onVoteUp(QString,QString,QString,QString)));
    }
    connect(client, SIGNAL(removeUser(PollsClient*)), this, SLOT(onRemoveUser(PollsClient*)));
    _clients.append(client);
}

void PollsServer::onRemoveUser(PollsClient *client)
{
    _clients.removeAt(_clients.indexOf(client));
}

bool PollsServer::isCodeAlreadyUsed(QString category, QString code) {
    return _dialog->isCodeAlreadyUsed(category, code);
}
