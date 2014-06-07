#include "pollsclient.h"
#include <QTcpServer>
#include <QMessageBox>
#include <QCryptographicHash>
#include "../Server/pollsserver.h"
#include "logger.h"

bool VerifyCode(QString __data, unsigned hash_length = 10) {
    QByteArray data(__data.toLocal8Bit());

    auto partToHash = data.left(hash_length);
    auto expectedHash = data.mid(hash_length, hash_length);

    QCryptographicHash hasher(QCryptographicHash::Sha256);
    hasher.addData(partToHash);
    auto hash = hasher.result().toHex();
    auto hashPart = hash.mid(1,hash_length);

//    qDebug() << "data: " << data << "\t part: " << partToHash << "\t hash_part: " << hashPart << "\t expected: " << expectedHash << "\t hash" << hash;
    return hashPart == expectedHash;
}


PollsClient::PollsClient(qintptr desc, PollsServer *srv, QObject *parent) :
    QObject(parent)
{
    _blockSize = 0;
    _isAuthed = false;

    _srv = srv;
    _sok = new QTcpSocket(this);
    _sok->setSocketDescriptor(desc);

    connect(_sok, SIGNAL(connected()), this, SLOT(onConnect()));
    connect(_sok, SIGNAL(disconnected()), this, SLOT(onDisconnect()));
    connect(_sok, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    connect(_sok, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(onError(QAbstractSocket::SocketError)));

    qDebug() << "Client connected" << desc;

    pingTimer = new QTimer(this);
    this->connect(pingTimer, SIGNAL(timeout()), SLOT(onTimerStop()));
    pingTimer->setInterval(1000);

    pingTimer->setSingleShot(false);
    pingTimer->start();
}

void PollsClient::onDisconnect() {
    qDebug() << "Client disconnected";
    //если авторизован
    if (_isAuthed)
    {
        //убирием из интерфейса
        emit removeUserFromGui(_name);
        //убираем из списка
        emit removeUser(this);
        pingTimer->stop();
    }
    deleteLater();
}

void PollsClient::onError(QAbstractSocket::SocketError socketError) const
{
    //w нужна для обсвобождения памяти от QMessageBox (посредством *parent = &w)
    QWidget w;
    switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
        break;
    case QAbstractSocket::HostNotFoundError:
        QMessageBox::information(&w, "Error", "The host was not found");
        break;
    case QAbstractSocket::ConnectionRefusedError:
        QMessageBox::information(&w, "Error", "The connection was refused by the peer.");
        break;
    default:
        QMessageBox::information(&w, "Error", "The following error occurred: "+_sok->errorString());
    }
    //тут вызовутся деструктор w и соответственно QMessageBox (по правилам с++)
}

void PollsClient::onTimerStop()
{
    doSendCommand(comPing);
}

void PollsClient::doSendCommand(qint8 comm)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (qint64)0;
    out << comm;
    out.device()->seek(0);
    out << (qint64)(block.size() - sizeof(qint64));
    _sok->write(block);

    if (quint8(comm) != ProtocolCommand::comPing)
        Logger::info("Command " + ReadableProtocolCommand(static_cast<ProtocolCommand>(comm)) + " sent to " + _name);
}

QString PollsClient::peerName()
{
    if (_peerName.isEmpty()) {
        _peerName = QHostInfo::fromName(_sok->peerAddress().toString()).hostName();
    }
    return _peerName;
}

void PollsClient::onConnect()
{
    //never calls, socket already connected to the tcpserver
    //we just binding to this socket here: _sok->setSocketDescriptor(desc);
}

void PollsClient::onReadyRead()
{
    QDataStream in(_sok);
    //если считываем новый блок первые 2 байта это его размер
    if (_blockSize == 0) {
        //если пришло меньше 2 байт ждем пока будет 2 байта
        if (_sok->bytesAvailable() < (qint64)sizeof(qint64))
            return;
        //считываем размер (2 байта)
        in >> _blockSize;
        qDebug() << "_blockSize now " << _blockSize;
    }
    //ждем пока блок прийдет полностью
    if (_sok->bytesAvailable() < _blockSize)
        return;
    else
        //можно принимать новый блок
        _blockSize = 0;
    //3 байт - команда серверу
    quint8 command;
    in >> command;
    ProtocolCommand protocolCommand = static_cast<ProtocolCommand>(command);
    QString cmdstr = ReadableProtocolCommand(protocolCommand);
    Logger::info("Received command " + cmdstr);
    qDebug() << "Received command " << command;
    //для неавторизованный пользователей принимается только команда "запрос на авторизацию"
    if (!_isAuthed && command != ProtocolCommand::comAuthRequest)
        return;

    switch(command)
    {
    //запрос на авторизацию
    case ProtocolCommand::comAuthRequest:
    {
        auto name =
                QString("%1 ( %2:%3 )")
                .arg(peerName())
                .arg(_sok->peerAddress().toString())
                .arg(QString::number(_sok->peerPort()));

        //авторизация пройдена
        _name = name;
        _isAuthed = true;
        //добавляем в интерфейс
        emit addUserToGui(name);
        doSendCommand(ProtocolCommand::comAuthSuccess);
    }
        break;
    case ProtocolCommand::comCodeVerify:
    {
        QString code, category;
        in >> code;
        in >> category;
        code = code.left(20);
        Logger::info("Received qr code '" + code + "'");
        qDebug() << "Received qr code '" << code << "'";
        if (VerifyCode(code)) {
            Logger::success("Code verified");
            qDebug() << "Code verified";

            if (!_srv->isCodeAlreadyUsed(category, code))
                doSendCommand(ProtocolCommand::comCodeVerified);
            else
                doSendCommand(ProtocolCommand::comCodeAlreadyUsed);
        } else {
            Logger::warning("Code not verified");
            doSendCommand(ProtocolCommand::comCodeNotVerified);
            qDebug() << "Code not verified";
        }
    }
        break;
    case ProtocolCommand::comVoteUp:
    {
        QString code, filename, category;
        in >> code;
        in >> filename;
        in >> category;
        code = code.left(20);

        Logger::info("Vote up for '" + filename + "' with code '" + code + "'");

        if (VerifyCode(code)) {
            Logger::success("Code verified");

            if (!_srv->isCodeAlreadyUsed(category, code)) {
                doSendCommand(ProtocolCommand::comCodeVerified);
                emit doVoteUp(category, code, filename, peerName());
                Logger::success("Vote accepted");
            } else {
                Logger::warning("Code already used");
                doSendCommand(ProtocolCommand::comCodeAlreadyUsed);
            }
        } else {
            Logger::warning("Code not verified");
            doSendCommand(ProtocolCommand::comCodeNotVerified);
        }
    }
    default:
        break;
    }
}
